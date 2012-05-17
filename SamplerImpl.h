
namespace DNest3
{

template<class ModelType>
Sampler<ModelType>::Sampler(const Options& options)
:options(options)
,particles(options.numParticles)
,logL(options.numParticles)
,indices(options.numParticles, 0)
,logLKeep(0)
{

}

} // namespace DNest3

