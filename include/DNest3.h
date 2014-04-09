/*
 * DNest3 C API
 */

#ifdef __cplusplus

#include <ostream>
#include <Model.h>
#include <Level.h>
#include <Options.h>
#include <LikelihoodType.h>
#include <Sampler.h>

using namespace std;
using namespace DNest3;

class CModel;

typedef vector<Level> Levels;
typedef Sampler<CModel> CSampler;

#else

typedef void CModel;
typedef void Level;
typedef void Levels;
typedef void LikelihoodType;
typedef void Options;
typedef void ostream;
typedef void CSampler;

#endif

#ifdef __cplusplus
extern "C" {
#endif

  /* Data structure for storing call-back functions (written in C) for the Model.*/
  typedef void (*CModelMethodType_void_CModel)(void*);
  typedef double (*CModelMethodType_double_CModel)(void*);
  typedef double (*CModelMethodType_double_const_CModel)(const void*);  
  typedef void (*CModelMethodType_void_CModel_ostream)(const void*,void*);
  typedef double (*CModelMethodType_double_CModel_CModel_double)(void*,const void*,double);
  typedef void (*CModelMethodType_void_CModel_CModel)(void*,const void*);
  typedef char* (*CModelMethodType_charp_const_CModel)(const void*);
  typedef struct 
  {
    CModelMethodType_void_CModel fromPrior;
    CModelMethodType_double_CModel perturb;
    CModelMethodType_double_const_CModel logLikelihood;
    CModelMethodType_void_CModel_ostream print;
    CModelMethodType_void_CModel_CModel copy;
    CModelMethodType_void_CModel delete_model;
    CModelMethodType_charp_const_CModel description;
  } CModelMethods;

  /* LikelihoodType */
   
  /* Constructors*/
  LikelihoodType* LikelihoodType_new1();
  LikelihoodType* LikelihoodType_new2(double logL,double tieBreaker);
  /* Destructor*/
  void LikelihoodType_delete(LikelihoodType* self);
  /* Operator*/
  int LikelihoodType_lt(LikelihoodType* self,const LikelihoodType* other);
  
  /* Options */

  /*Constructors*/
  Options* Options_new(int numparticles,int newLevelInterval,int saveInterval,int threadSteps,int maxNumLevels,double lambda,double beta,int maxNumSamples);

  /*Destructor*/
  void Options_delete(Options* self);

  /* Level */

  /*Constructors*/
  Level* Level_new1(double logX,const LikelihoodType* cutoff);
  Level* Level_new2(double logX,double logL,double tieBreaker);
  /*Destructors*/
  void Level_delete(Level* self);
  /*Operators*/
  Level* Level_plusequals(Level* self,const Level* other);
  Level* Level_minusequals(Level* self,const Level* other);
  /*Methods*/
  LikelihoodType Level_get_cutoff(Level* self);
  double Level_get_logX(Level* self);
  long Level_get_tries(Level* self);
  void Level_incrementVisits(Level* self,int incrementExceeds);
  void Level_incrementTries(Level* self,int accepted);
  void Level_renormaliseVisits1(Level* self,int regularisation);
  void Level_recalculateLogX(Level* self,Levels* levels,double compression,int regularisation);
  void Level_renormaliseVisits2(Level* self,Levels* levels,int regularisation);
  void Level_loadLevels(const Level* self,Levels* out,const char* filename);
  
  /* Levels */
  /*Constructor*/
  Levels* Levels_new();
  /*Destructor*/
  void Levels_delete(Levels* self);
  /*Methods*/
  int Levels_size(Levels* self);
  void Levels_push_back(Levels* self,Level* a);
  Level Levels_get(Levels* self,int i);

  /*Model*/
  /*Constructor*/
CModel* CModel_new(
    CModelMethods* cbfuncs,
    void* init_state   
);

  void CModel_delete(CModel* self);

  /*RandomNumberGenerator*/
  void RandomNumberGenerator_initialise_instance();
  void RandomNumberGenerator_set_seed(long seed);

  /*Sampler*/
  /*Constructor*/
  CSampler* Sampler_new(const CModel* factory,double compression,const Options* options);
  void Sampler_delete(CSampler* self);
  void Sampler_loadLevels(CSampler* self,const char* filename);
  void Sampler_run(CSampler* self);

  /** Misc. utilities **/
  ostream* create_ostream_handle_for_file(const char* filename);
  void print_string_to_stream(ostream* out,const char* string);
  
  
#ifdef __cplusplus
}
#endif
