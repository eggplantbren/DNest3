namespace DNest3
{

template<class ModelType>
MTSampler<ModelType>::MTSampler(int numThreads, const Options& options)
:samplers(numThreads, Sampler<ModelType>(options))
{

}

} // namespace DNest3

