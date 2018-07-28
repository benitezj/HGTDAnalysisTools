void  pileupParameters(int mu=100){
  float R=3.223656e+02;
  float numberOfLowPtMinBias = mu / ( 1. + 1./R);
  float numberOfHighPtMinBias = mu / ( 1. + R);
  cout<<" numberOfLowPtMinBias="<<numberOfLowPtMinBias<<"  numberOfHighPtMinBias="<<numberOfHighPtMinBias<<endl;
}
