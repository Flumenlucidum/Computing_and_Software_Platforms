library(CondIndTests)
library(mice)
library(missForest)
library(VIM)
library(momentchi2)
library(Rcpp)
library(tictoc)
#nd<-read.csv('final_ND_matrix.csv')

#aggr(nd_test, cex.axis=.7, gap=3, ylab=c("Missing data","Pattern"))
#nd_imp<-mice(nd_test)
#nd.comp<-complete(nd_imp,3)
nd.comp<-read.csv('nd_comp.csv',h=F)
#RCIT functions --Eric. V Strobl
KCIT <- function(x,y,z,Bootstrap=TRUE){
  
  if( length(z)==0 || missing(z) ){
    p_val = U_KCI(x,y);
    #print(p_val)
    return(p_val)
  }
  else{
    
    x=normalize(x);
    y=normalize(y);
    z=normalize(z);
    
    T=length(x);
    
    lambda = 1E-3;
    Thresh = 1E-5;
    
    if (T <= 200) {
      #width = 0.8;
      width = 1.2;
    } else if (T > 1200){
      #width = 0.3;
      width = 0.7;
    }else{
      #width = 0.5;
      width = 0.4;
    }
    
    if (length(x)>500){
      width = median(as.vector(dist(cbind(x[1:500],y[1:500]))));}
    else {
      width = median(as.vector(dist(cbind(x,y))));}
    
    if (is.null(dim(z)[2])) { D = 1;}
    else{D = dim(z)[2];}
    
    theta = 1/(width^2 * D);
    
    H =  diag(T) - (1/T)*matrix(1,T,T);
    
    Kx = RBF_kernel(cbind(x, z/2), theta); Kx = H %*% Kx %*% H;
    
    Ky = RBF_kernel(y, theta); Ky = H %*% Ky %*% H;
    
    Kz = RBF_kernel(z, theta); Kz = H %*% Kz %*% H;
    
    #P1 = (diag(T)-Kz %*% chol2inv(chol(Kz + lambda*diag(T))));
    P1 = (diag(T)-Kz %*% solve(Kz + lambda*diag(T)));
    Kxz = P1 %*% Kx %*% t(P1);
    Kyz = P1 %*% Ky %*% t(P1);
    
    Sta = sum(diag(Kxz %*% Kyz));
    
    df = sum(diag(diag(T)-P1));
    
    listxz = eigen((Kxz+t(Kxz))/2,symmetric=TRUE);
    eig_Kxz=listxz[[1]]; eivx=listxz[[2]]
    
    listyz = eigen((Kyz+t(Kyz))/2,symmetric=TRUE);
    eig_Kyz=listyz[[1]]; eivy=listyz[[2]]
    
    
    IIx = which(eig_Kxz > max(eig_Kxz) * Thresh);
    IIy = which(eig_Kyz > max(eig_Kyz) * Thresh);
    eig_Kxz = eig_Kxz[IIx];
    eivx = eivx[,IIx];
    eig_Kyz = eig_Kyz[IIy];
    eivy = eivy[,IIy];
    
    eiv_prodx = eivx %*% diag(sqrt(eig_Kxz));
    eiv_prody = eivy %*% diag(sqrt(eig_Kyz));
    
    Num_eigx = dim(eiv_prodx)[2];
    Num_eigy = dim(eiv_prody)[2];
    Size_u = Num_eigx * Num_eigy;
    
    uu = matrix(0,T,Size_u);
    
    
    for (i in 1:Num_eigx){
      for (j in 1:Num_eigy){
        uu[,(i-1)*Num_eigy + j] = eiv_prodx[,i] * eiv_prody[,j];
      }
    }
    
    if (Size_u > T){
      uu_prod = uu %*% t(uu);}
    else{
      uu_prod = t(uu) %*% uu;
    }
    
    if (Bootstrap){
      T_BS=5000;
      IF_unbiased=TRUE;
      
      list_uu = eigen(uu_prod);
      eig_uu =list_uu[[1]];
      II_f = which(eig_uu > max(eig_uu) * Thresh);
      eig_uu = eig_uu[II_f];
      
      if (length(eig_uu)*T < 1E6){
        f_rand1 = matrix(rnorm(length(eig_uu)*T_BS)^2,length(eig_uu),T_BS);
        Null_dstr = t(eig_uu) %*% f_rand1;
        
      } else {
        
        Null_dstr = matrix(0,1,T_BS);
        Length = max(c(floor(1E6/T),100));
        Itmax = floor(length(eig_uu)/Length);
        for (iter in 1:Itmax){
          f_rand1 = matrix(rnorm(Length*T_BS)^2,Length,T_BS);
          Null_dstr = Null_dstr + t(eig_uu[((iter-1)*Length+1):(iter*Length)]) %*% f_rand1;
          
        }
      }
      
      sort_Null_dstr = sort(Null_dstr);
      #Cri = sort_Null_dstr[ceiling((1-alpha)*T_BS)];
      p_val = sum(Null_dstr>Sta)/T_BS;
      #print(p_val)
      return(p_val)
      
    } else {
      mean_appr = sum(diag(uu_prod));
      var_appr = 2*sum(diag(uu_prod^2));
      k_appr = mean_appr^2/var_appr;
      theta_appr = var_appr/mean_appr;
      p_val = 1-pgamma(Sta, shape=k_appr, scale=theta_appr);
      #print(p_val)
      return(p_val)
      
    }
    
  }
  
}

RBF_kernel <- function(x,width){
  
  n2=(dist(cbind(x,x),diag = TRUE, upper = TRUE))^2;
  n2=as.matrix(n2);
  
  width=width/2;
  kx = exp(-n2*width);
  
  return(kx)
  
}

RCIT <- function(x,y,z=NULL,approx="lpd4",num_f=100,num_f2=5, seed=NULL){
  
  if (length(z)==0){
    out=RIT(x,y,approx=approx,seed=seed);
    return(out)
  }
  else{
    tic('normalize')
    x=matrix2(x);
    y=matrix2(y);
    z=matrix2(z);
    
    z=z[,apply(z,2,sd)>0];
    z=matrix2(z);
    d=ncol(z);
    
    if (length(z)==0){
      out=RIT(x,y,approx=approx, seed=seed);
      return(out);
    } else if (sd(x)==0 | sd(y)==0){
      out=list(p=1,Sta=0);
      return(out)
    }
    
    r=nrow(x);
    if (r>500){
      r1=500
    } else {r1=r;}
    
    x=normalize(x);
    y=normalize(y);
    z=normalize(z);
    
    #for (t in seq_len(ncol(x))){
    #  x[,t] = pnorm(ecdf(x[,t])(x[,t]));
    #}
    #for (t in seq_len(ncol(y))){
    #  y[,t] = pnorm(ecdf(y[,t])(y[,t]));
    #}
    #for (t in seq_len(d)){
    #  z[,t] = pnorm(ecdf(z[,t])(z[,t]));
    #}
    
    
    y = cbind(y,z)
    toc()
    tic('rand_four') 
    
    four_z = random_fourier_features(z[,1:d],num_f=num_f,sigma=median(c(t(dist(z[1:r1,])))), seed = seed );
    four_x = random_fourier_features(x,num_f=num_f2,sigma=median(c(t(dist(x[1:r1,])))), seed = seed );
    four_y = random_fourier_features(y,num_f=num_f2,sigma=median(c(t(dist(y[1:r1,])))), seed = seed );
    
    #f_x=normalize(four_x$feat);
    #f_y=normalize(four_y$feat);
    #f_z=normalize(four_z$feat);
    f_x=normalize(four_x);
    f_y=normalize(four_y);
    f_z=normalize(four_z);
    toc()
    
    tic('Cxy Czz')
    
    Cxy=cov(f_x,f_y);
    
    Czz = cov(f_z);
   
    i_Czz = chol2inv(chol( Czz + diag(num_f) * 1e-10))
    # i_Czz = ginv(Czz+diag(num_f)*1E-10); #requires library(MASS)
    Cxz=cov(f_x,f_z);
    Czy=cov(f_z,f_y);
    
    toc()
    tic('matmul')

    if(!is.loaded('mat_mul'))
    {
	dyn.load('rand_four.so')
    }
   

    temp<-.C('mat_mul',as.double(f_z),as.double(i_Czz),C=double(length=r*100),as.integer(r),as.integer(num_f),as.integer(num_f))

    z_i_Czz=matrix(temp[['C']],r,100)
    
    temp<-.C('mat_mul',as.double(z_i_Czz),as.double(t(Cxz)),C=double(length=r*5),as.integer(r),as.integer(num_f2),as.integer(num_f))

     e_x_z=matrix(temp[['C']],r,5)
     

     temp<-.C('mat_mul',as.double(z_i_Czz),as.double(Czy),C=double(length=r*5),as.integer(r),as.integer(num_f2),as.integer(num_f))

        e_y_z=matrix(temp[['C']],r,5)
    #z_i_Czz=f_z%*%i_Czz;
    #e_x_z = z_i_Czz%*%t(Cxz);
    #e_y_z = z_i_Czz%*%Czy;
    toc()
    #approximate null distributions
    
    res_x = f_x-e_x_z;
    res_y = f_y-e_y_z;
    
    if (num_f2==1){
      approx="hbe"
    }
    
    if (approx == "perm"){
      
      Cxy_z = cov(res_x, res_y);
      Sta = r*sum(Cxy_z^2);
      
      nperm =1000;
      
      Stas = c();
      for (ps in 1:nperm){
        perm = sample(1:r,r);
        Sta_p = Sta_perm(res_x[perm,],res_y,r)
        Stas = c(Stas, Sta_p);
        
      }
      
      p = 1-(sum(Sta >= Stas)/length(Stas));
      
    } else {
      
      Cxy_z=Cxy-Cxz%*%i_Czz%*%Czy; #less accurate for permutation testing
      Sta = r*sum(Cxy_z^2);
      
      d =expand.grid(1:ncol(f_x),1:ncol(f_y));
      res = res_x[,d[,1]]*res_y[,d[,2]];
      Cov = 1/r * (t(res)%*%res);
      
      if (approx == "chi2"){
        i_Cov = ginv(Cov)
        
        Sta = r * (c(Cxy_z)%*%  i_Cov %*% c(Cxy_z) );
        p = 1-pchisq(Sta, length(c(Cxy_z)));
      } else{
        tic('eigen')
        eig_d = eigen(Cov,symmetric=TRUE);
        eig_d$values=eig_d$values[eig_d$values>0];
        
        if (approx == "gamma"){
          p=1-sw(eig_d$values,Sta);
          
        } else if (approx == "hbe") {
          
          p=1-hbe(eig_d$values,Sta);
          
        } else if (approx == "lpd4"){
          eig_d_values=eig_d$values;
          p=try(1-lpb4(eig_d_values,Sta),silent=TRUE);
          if (!is.numeric(p) | is.nan(p)){
            p=1-hbe(eig_d$values,Sta);
          }
          toc()
        }
      }
    }
    
    if (p<0) p=0;
    
    out=list(p=p,Sta=Sta);
    return(out)
  }

}

RCoT <- function(x,y,z=NULL,approx="lpd4",num_f=100,num_f2=5,seed=NULL){
  
  if (length(z)==0){
    out=RIT(x,y,approx=approx,seed=seed);
    return(out)
  }
  else{
    
    x=matrix2(x);
    y=matrix2(y);
    z=matrix2(z);
    
    z=z[,apply(z,2,sd)>0];
    z=matrix2(z);
    d=ncol(z);
    
    if (length(z)==0){
      out=RIT(x,y,approx=approx, seed=seed);
      return(out);
    } else if (sd(x)==0 | sd(y)==0){
      out=list(p=1,Sta=0);
      return(out)
    }
    
    r=nrow(x);
    if (r>500){
      r1=500
    } else {r1=r;}
    
    x=normalize(x);
    y=normalize(y);
    z=normalize(z);
    
    #for (t in seq_len(ncol(x))){
    #  x[,t] = pnorm(ecdf(x[,t])(x[,t]));
    #}
    #for (t in seq_len(ncol(y))){
    #  y[,t] = pnorm(ecdf(y[,t])(y[,t]));
    #}
    #for (t in seq_len(d)){
    #  z[,t] = pnorm(ecdf(z[,t])(z[,t]));
    #}
    
    
    four_z = random_fourier_features(z[,1:d],num_f=num_f,sigma=median(c(t(dist(z[1:r1,])))), seed = seed );
    four_x = random_fourier_features(x,num_f=num_f2,sigma=median(c(t(dist(x[1:r1,])))), seed = seed );
    four_y = random_fourier_features(y,num_f=num_f2,sigma=median(c(t(dist(y[1:r1,])))), seed = seed );
    
    f_x=normalize(four_x$feat);
    f_y=normalize(four_y$feat);
    f_z=normalize(four_z$feat);
    
    Cxy=cov(f_x,f_y);
    
    Czz = cov(f_z);
    
    i_Czz = chol2inv(chol( Czz + diag(num_f) * 1e-10))
    # i_Czz = ginv(Czz+diag(num_f)*1E-10); #requires library(MASS)
    Cxz=cov(f_x,f_z);
    Czy=cov(f_z,f_y);
    
    z_i_Czz=f_z%*%i_Czz;
    e_x_z = z_i_Czz%*%t(Cxz);
    e_y_z = z_i_Czz%*%Czy;
    
    #approximate null distributions
    
    res_x = f_x-e_x_z;
    res_y = f_y-e_y_z;
    
    if (num_f2==1){
      approx="hbe"
    }
    
    if (approx == "perm"){
      
      Cxy_z = cov(res_x, res_y);
      Sta = r*sum(Cxy_z^2);
      
      nperm =1000;
      
      Stas = c();
      for (ps in 1:nperm){
        perm = sample(1:r,r);
        Sta_p = Sta_perm(res_x[perm,],res_y,r)
        Stas = c(Stas, Sta_p);
        
      }
      
      p = 1-(sum(Sta >= Stas)/length(Stas));
      
    } else {
      
      Cxy_z=Cxy-Cxz%*%i_Czz%*%Czy; #less accurate for permutation testing
      Sta = r*sum(Cxy_z^2);
      
      d =expand.grid(1:ncol(f_x),1:ncol(f_y));
      res = res_x[,d[,1]]*res_y[,d[,2]];
      Cov = 1/r * (t(res)%*%res);
      
      if (approx == "chi2"){
        i_Cov = ginv(Cov)
        
        Sta = r * (c(Cxy_z)%*%  i_Cov %*% c(Cxy_z) );
        p = 1-pchisq(Sta, length(c(Cxy_z)));
      } else{
        
        eig_d = eigen(Cov,symmetric=TRUE);
        eig_d$values=eig_d$values[eig_d$values>0];
        
        if (approx == "gamma"){
          p=1-sw(eig_d$values,Sta);
          
        } else if (approx == "hbe") {
          
          p=1-hbe(eig_d$values,Sta);
          
        } else if (approx == "lpd4"){
          eig_d_values=eig_d$values;
          p=try(1-lpb4(eig_d_values,Sta),silent=TRUE);
          if (!is.numeric(p) | is.nan(p)){
            p=1-hbe(eig_d$values,Sta);
          }
        }
      }
    }
    
    if (p<0) p=0;
    
    out=list(p=p,Sta=Sta);
    return(out)
  }
  
}

RIT <- function(x,y,approx="lpd4",seed=NULL){
  
  if (sd(x,na.rm=T)==0 | sd(y,na.rm=T)==0){
    out=list(p=1,Sta=0,w=w,b=b);
    return(out$p)
  }
  
  x=matrix2(x);
  y=matrix2(y);
  
  
  r=nrow(x);
  if (r>500){
    r1=500
  } else {r1=r;}
  
  x=normalize(x);
  y=normalize(y);
  
  four_x = random_fourier_features(x,num_f=5,sigma=median(c(t(dist(x[1:r1,])))), seed = seed );
  four_y = random_fourier_features(y,num_f=5,sigma=median(c(t(dist(y[1:r1,])))), seed = seed );
  
  f_x=normalize(four_x); #stabilizes computations
  f_y=normalize(four_y); #stabilizes computations
  
  Cxy=cov(f_x,f_y);
  
  Sta = r*sum(Cxy^2);
  
  #approximate null distributions
  
  if (approx == "perm"){
    nperm =1000;
    
    Stas = c();
    for (ps in 1:nperm){
      perm = sample(1:r,r);
      Sta_p = Sta_perm(f_x[perm,],f_y,r)
      Stas = c(Stas, Sta_p);
      
    }
    
    p = 1-(sum(Sta >= Stas)/length(Stas));
    
  } else{
    
    res_x = f_x-repmat(t(matrix(colMeans(f_x))),r,1);
    res_y = f_y-repmat(t(matrix(colMeans(f_y))),r,1);
    
    d =expand.grid(1:ncol(f_x),1:ncol(f_y));
    res = res_x[,d[,1]]*res_y[,d[,2]];
    Cov = 1/r * (t(res)%*%res);
    
    if (approx == "chi2"){
      i_Cov = ginv(Cov)
      
      Sta = r * (c(Cxy)%*%  i_Cov %*% c(Cxy) );
      p = 1-pchisq(Sta, length(c(Cxy)));
    } else{
      
      eig_d = eigen(Cov);
      eig_d$values=eig_d$values[eig_d$values>0];
      
      if (approx == "gamma"){
        p=1-sw(eig_d$values,Sta);
        
      } else if (approx == "hbe") {
        
        p=1-hbe(eig_d$values,Sta);
        
      } else if (approx == "lpd4"){
        eig_d_values=eig_d$values;
        p=try(1-lpb4(eig_d_values,Sta), silent=TRUE);
        if (!is.numeric(p)  | is.nan(p)){
          p=1-hbe(eig_d$values,Sta);
        }
      }
    }
  }
  
  if (p<0) p=0;
  
  out=list(p=p,Sta=Sta);
  return(out)
}

Sta_perm <- function(f_x,f_y,r){
  
  Cxy=cov(f_x,f_y);
  
  Sta = r*sum(Cxy^2);
  
  return(Sta)
  
}

U_KCI <-function(x,y){
  
  if (sd(x,na.rm=T)>0) x=RCIT:::normalize(x);
  if (sd(y,na.rm=T)>0) y=RCIT:::normalize(y);
  
  
  T=length(x);
  
  if (length(x)>500){
    width = sqrt(2)*median(as.vector(dist(cbind(x[1:500],y[1:500]))));}
  else {
    width = sqrt(2)*median(as.vector(dist(cbind(x,y))));}
  
  theta = 1/(width^2);
  
  H =  diag(T) - (1/T)*matrix(1,T,T);
  
  Kx = RBF_kernel(x, theta); Kx = H %*% Kx %*% H;
  Ky = RBF_kernel(y, theta); Ky = H %*% Ky %*% H;
  
  Sta = sum(diag(Kx %*% Ky));
  
  mean_appr = sum(diag(Kx)) * sum(diag(Ky)) /T;
  var_appr = 2* sum(diag(Kx %*% Kx)) %*% sum(diag(Ky %*% Ky))/T^2;
  k_appr = (mean_appr^2)/var_appr;
  theta_appr = var_appr/mean_appr;
  
  
  p_val = 1-pgamma(Sta, shape=k_appr, scale=theta_appr);
  
  return(p_val)
  
}

matrix2 <- function(mat)
{
  if (is.matrix(mat)) {
    return(mat);
  } else {
    mat = matrix(mat);
    return(mat); }
}

normalize <- function(mat){
  
  if (is.null(nrow(mat))){mat = matrix(mat);}
  
  mat = apply(mat, 2, function(x) if (sd(x,na.rm=T)>0){(x - mean(x,na.rm=T)) / sd(x,na.rm=T)} else{x-mean(x,na.rm=T);})
  
  
}

random_fourier_features <- function(x,w=NULL,b=NULL,num_f=NULL,sigma=NULL,seed=NULL){
  
  if (length(num_f)==0) num_f = 25;
  
  x = matrix2(x);
  
  r=nrow(x);
  c=ncol(x);
  
  if (sigma==0 | is.na(sigma)){sigma=1};
  
  if (length(w)==0){
    if (length(sigma)==0){sigma=1;}
    set.seed(seed)
    w=(1/sigma)*matrix(rnorm(num_f*c),num_f,c);
    set.seed(seed)
    b=matrix(2*pi*runif(num_f),num_f,1);
  } #else if (nrow(w)<num_f){
  #w=rbind(w,(1/sigma)*matrix(rnorm((num_f-nrow(w))*c),num_f-nrow(w),c));
  #b=rbind(b,repmat(2*pi*runif(num_f-nrow(b)),1,r));
  #}
  
  #feat = sqrt(2)*t(cos(w[1:num_f,1:c]%*%t(x)+b[1:num_f,]));
  #feat = sqrt(2)*t(log(1+exp(w[1:num_f,1:c]%*%t(x)+b[1:num_f,])));
  if(!is.loaded('rand_four'))
  {
  	dyn.load('rand_four.so')
  }
  
  
  x=t(x)
  feat_pre<-.C('rand_four',as.double(w),as.double(x),as.double(b),result=double(length=num_f*r),as.integer(num_f),as.integer(c),as.integer(r))
  
  feat<-t(matrix(feat_pre[['result']],num_f,r))
  
  #feat=t(feat)
  #feat from gpu 
  #out=list(feat=feat,w=w,b=b);
  return(feat)
}

repmat = function(X,m,n){
  ##R equivalent of repmat (matlab)
  X=matrix2(X);
  mx = dim(X)[1]
  nx = dim(X)[2]
  matrix(t(matrix(X,mx,nx*n)),mx*m,nx*n,byrow=T)
}

#library(foreach)
#library(doParallel)
#detectCores()
#myCluster <- makeCluster(15, # number of cores to use
 #                        type = "PSOCK") # type of cluster
#registerDoParallel(myCluster)



#initial adjacency matrix
RCIT_mat<-matrix(1,13,13)
for (i in 1:13){
  RCIT_mat[i,i]<-0
}
colnames(RCIT_mat)<-colnames(nd.comp)
rownames(RCIT_mat)<-colnames(nd.comp)

#Level 0
#without parallel core 
for(i in 1:13){
  first<-nd.comp[,i]
  for(j in which(RCIT_mat[i,]!=0)){

    k<-RCIT(first,nd.comp[,j])
    if (k$p>0.05){
        RCIT_mat[i,j]<-0
        RCIT_mat[j,i]<-0
      }
  print(i)
  print(j)
  }
}
print('level0')
print(RCIT_mat)
#with parallelism 
#for(i in 1:13){
 
 # first<-nd.comp[,i]

  #result<-foreach(j=which(RCIT_mat[i,]!=0),.combine=cbind,.packages = 'RCIT')%dopar%{
    
   # k<-RCIT(first,nd.comp[,j])
   # c(j,k$p)
 # }
  #for(j in 1:ncol(result))
  #{
   # if (result[2,j]>0.05){
    #  RCIT_mat[i,result[1,j]]<-0
     # RCIT_mat[result[1,j],i]<-0
   # }
   # print(i)
   # print(result[1,j])
  #}
#}

#Level 1 
#without parallelism 
for(i in 1:13){
  first<-nd.comp[,i]
  adj<-which(RCIT_mat[i,]!=0)
  for(j in adj){
    second<-nd.comp[,j]
    adj2=adj[which(adj!=j)]
    for(k in adj2){
        result<-RCIT(first,second,nd.comp[,k])
        if (result$p>0.05){
          RCIT_mat[i,j]<-0
          RCIT_mat[j,i]<-0
          break
        }
    }
  }
    print(i)
    print(j)
    print(k)
}

print('level1')
print(RCIT_mat)
#Level 2 
for(i in 1:13){
  first<-nd.comp[,i]
  adj<-which(RCIT_mat[i,]!=0)
  for(j in adj){
    second<-nd.comp[,j]
    adj2<-adj[which(adj!=j)]
    comb<-expand.grid(adj2,adj2)
    comb<-comb[which(comb$Var1<comb$Var2),]
    for(k in 1:nrow(comb)){
      con1<-comb[k,1]
      con2<-comb[k,2]
      result<-RCIT(first,second,cbind(nd.comp[1:nrow(nd.comp),con1],nd.comp[1:nrow(nd.comp),con2]))
      if (result$p>0.05){
        RCIT_mat[i,j]<-0
        RCIT_mat[j,i]<-0
        break
      }
    }
  }
  print(i)
  print(j)
}

print('level2')
print(RCIT_mat)
#Level 3
for(i in 1:13){
  first<-nd.comp[,i]
  adj<-which(RCIT_mat[i,]!=0)
  for(j in adj){
    second<-nd.comp[,j]
    adj2<-adj[which(adj!=j)]
    comb<-expand.grid(adj2,adj2,adj2)
    comb<-comb[which(comb$Var1<comb$Var2 & comb$Var2<comb$Var3),]
    if(nrow(comb)!=0 & length(adj2)>=3){
      for(k in 1:nrow(comb)){
        con1<-comb[k,1]
        con2<-comb[k,2]
        con3<-comb[k,3]
        print(i)
        print(j)
        print(con1)
        print(con2)
        print(con3)
        result<-RCIT(first,second,cbind(nd.comp[1:nrow(nd.comp),con1],nd.comp[1:nrow(nd.comp),con2],nd.comp[1:nrow(nd.comp),con3]))
        if (result$p>0.05){
          RCIT_mat[i,j]<-0
          RCIT_mat[j,i]<-0
          break
        }
      }
    }
  }
}

print('level3')
print(RCIT_mat)
#Level 4 
for(i in 1:13){
  first<-nd.comp[,i]
  adj<-which(RCIT_mat[i,]!=0)
  for(j in adj){
    second<-nd.comp[,j]
    adj2<-adj[which(adj!=j)]
    comb<-expand.grid(adj2,adj2,adj2,adj2)
    comb<-comb[which(comb$Var1<comb$Var2 & comb$Var2<comb$Var3 & comb$Var3<comb$Var4),]
    if(nrow(comb)!=0 & length(adj2)>=4){
      for(k in 1:nrow(comb)){
        con1<-comb[k,1]
        con2<-comb[k,2]
        con3<-comb[k,3]
        con4<-comb[k,4]
        print(i)
        print(j)
        print(con1)
        print(con2)
        print(con3)
        print(con4)
        result<-RCIT(first,second,cbind(nd.comp[1:nrow(nd.comp),con1],nd.comp[1:nrow(nd.comp),con2],nd.comp[1:nrow(nd.comp),con3],nd.comp[1:nrow(nd.comp),con4]))
        if (result$p>0.05){
          RCIT_mat[i,j]<-0
          RCIT_mat[j,i]<-0
          break
        }
      }
    }
  }
}
print('level4')
print(RCIT_mat)
#Level 5
for(i in 1:13){
  first<-nd.comp[,i]
  adj<-which(RCIT_mat[i,]!=0)
  for(j in adj){
    second<-nd.comp[,j]
    adj2<-adj[which(adj!=j)]
    comb<-expand.grid(adj2,adj2,adj2,adj2,adj2)
    comb<-comb[which(comb$Var1<comb$Var2 & comb$Var2<comb$Var3 & comb$Var3<comb$Var4 & comb$Var4<comb$Var5),]
    if(nrow(comb)!=0 & length(adj2)>=5){
      for(k in 1:nrow(comb)){
        con1<-comb[k,1]
        con2<-comb[k,2]
        con3<-comb[k,3]
        con4<-comb[k,4]
        con5<-comb[k,5]
        print(i)
        print(j)
        print(con1)
        print(con2)
        print(con3)
        print(con4)
        print(con5)
        result<-RCIT(first,second,cbind(nd.comp[1:nrow(nd.comp),con1],nd.comp[1:nrow(nd.comp),con2],nd.comp[1:nrow(nd.comp),con3],nd.comp[1:nrow(nd.comp),con4],nd.comp[1:nrow(nd.comp),con5]))
        if (result$p>0.05){
          RCIT_mat[i,j]<-0
          RCIT_mat[j,i]<-0
          break
        }
      }
    }
  }
}
print('level5')
print(RCIT_mat)
#Level 6
for(i in 1:13){
  first<-nd.comp[,i]
  adj<-which(RCIT_mat[i,]!=0)
  for(j in adj){
    second<-nd.comp[,j]
    adj2<-adj[which(adj!=j)]
    comb<-expand.grid(adj2,adj2,adj2,adj2,adj2,adj2)
    comb<-comb[which(comb$Var1<comb$Var2 & comb$Var2<comb$Var3 & comb$Var3<comb$Var4 & comb$Var4<comb$Var5 &comb$Var5<comb$Var6),]
    if(nrow(comb)!=0 & length(adj2)>=6){
      for(k in 1:nrow(comb)){
        con1<-comb[k,1]
        con2<-comb[k,2]
        con3<-comb[k,3]
        con4<-comb[k,4]
        con5<-comb[k,5]
        con6<-comb[k,6]
        print(i)
        print(j)
        print(con1)
        print(con2)
        print(con3)
        print(con4)
        print(con5)
        print(con6)
        result<-RCIT(first,second,cbind(nd.comp[1:nrow(nd.comp),con1],nd.comp[1:nrow(nd.comp),con2],nd.comp[1:nrow(nd.comp),con3],nd.comp[1:nrow(nd.comp),con4],nd.comp[1:nrow(nd.comp),con5],nd.comp[1:nrow(nd.comp),con6]))
        if (result$p>0.05){
          RCIT_mat[i,j]<-0
          RCIT_mat[j,i]<-0
          break
        }
      }
    }
  }
}

print('level6')
print(RCIT_mat)
#Done with the CPDAG
#Starts to make direction of the edges drawn

save(RCIT_mat,file='RCIT_mat.RData')


