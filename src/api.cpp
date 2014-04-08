/* capi.cpp
 * DNest3 C API.
 */

#include <iostream>
#include <fstream>

#include "LikelihoodType.h"
#include "Options.h"
#include "Level.h"
#include "Model.h"
#include "Sampler.h"
#include "RandomNumberGenerator.h"

#include "DNest3.h"

using namespace DNest3;

struct CModel : Model {

  
  CModel(const CModelMethods* cbfuncs,void* state):state(state),cbfuncs(cbfuncs) {}

  CModel(const CModel& other){
    cbfuncs = other.cbfuncs;        
    
    other.cbfuncs->copy(this->state,other.state);
  }

  ~CModel(){
    this->cbfuncs->delete_model(this->state);
  }

  void fromPrior(){
    this->cbfuncs->fromPrior(this->state);
  }

  double perturb(){
    return this->cbfuncs->perturb(this->state);
  }

  double logLikelihood() const{
    return this->cbfuncs->logLikelihood(this->state);
  }
  
  void print(std::ostream& out) const{
    this->cbfuncs->print(this->state,(void*)&out);
  }
  
  std::string description() const {
    return std::string("");
  }

  void* state=NULL;//place where data goes
  const CModelMethods* cbfuncs=NULL;//place where C defined methods go

  CModel& operator=(const CModel& other){
    cbfuncs = other.cbfuncs;
    
    other.cbfuncs->copy(state,other.state);
    
    return *this;
  }
};
  
/* Template for generating a simple C API for std::vector<T>
 * Adapted from http://stackoverflow.com/questions/16885344/how-to-handle-c-return-type-stdvectorint-in-python-ctypes .
 */

template <typename T>
std::vector<T>* new_vector(){
  return new std::vector<T>;
}

template <typename T>
void delete_vector(std::vector<T>* v){
  delete v;
}

template <typename T>
int vector_size(std::vector<T>* v){
  return v->size();
}

template <typename T>
T vector_get(std::vector<T>* v, int i){
  return v->at(i);
}

template <typename T>
void vector_push_back(std::vector<T>* v,T a){
   v->push_back(a);
}

/* 
 * Functions visible to C...
 */
  
/* LikelihoodType */
   
// Constructors
LikelihoodType* LikelihoodType_new1(){return new LikelihoodType();}
LikelihoodType* LikelihoodType_new2(double logL,double tieBreaker){return new LikelihoodType(logL,tieBreaker);}

// Destructor
void LikelihoodType_delete(LikelihoodType* self){
  delete self;
}

// Operator
int LikelihoodType_lt(LikelihoodType* self,const LikelihoodType* other){return *self<*other;}
  
/* Options */

//Constructors
Options* Options_new(int numparticles,int newLevelInterval,int saveInterval,int threadSteps,int maxNumLevels,double lambda,double beta,int maxNumSamples){
  return new Options(numparticles,newLevelInterval,saveInterval,threadSteps,maxNumLevels,lambda,beta,maxNumSamples);
}

//Destructor
void Options_delete(Options* self){
  delete self;
}

/* Level */

//Constructors
Level* Level_new1(double logX,const LikelihoodType* cutoff){return new Level(logX,*cutoff);}
Level* Level_new2(double logX,double logL,double tieBreaker){return new Level(logX,logL,tieBreaker);}
//Destructors
void Level_delete(Level* self){
  delete self;
}

LikelihoodType Level_get_cutoff(Level* self){
  return self->get_cutoff();
}

double Level_get_logX(Level* self){
  return self->get_logX();
}

long Level_get_tries(Level* self){
  return self->get_tries();
}

void Level_incrementVisits(Level* self,int incrementExceeds){
  self->incrementVisits(bool(incrementExceeds));
}

void Level_incrementTries(Level* self,int accepted){
  self->incrementTries(bool(accepted));
}

void Level_renormaliseVisits1(Level* self,int regularisation){
  self->renormaliseVisits(regularisation);
}

void Level_recalculateLogX(Level* self,Levels* levels,double compression,int regularisation){
  self->recalculateLogX(*levels,compression,regularisation);
}

void Level_renormaliseVisits2(Level* self,Levels* levels,int regularisation){
  self->renormaliseVisits(*levels,regularisation);
}

void Level_loadLevels(const Level* self,Levels* out,const char* filename){
   *out=self->loadLevels(filename);
}

//Operators
Level* Level_plusequals(Level* self,const Level* other){
  *self+=*other;
  return self;
}

Level* Level_minusequals(Level* self,const Level* other){
  *self-=*other;
  return self;
}

/* Levels */

//Constructor
Levels* Levels_new(){
  return new_vector<Level>();
}

//Destructor
void Levels_delete(Levels* self){
  delete_vector<Level>(self);
}

//Methods
int Levels_size(Levels* self){
  return vector_size<Level>(self);
}

Level Levels_get(Levels* self,int i){
  return vector_get<Level>(self,i);
}

void Levels_push_back(Levels* self,Level* a){
  vector_push_back<Level>(self,*a);
}

/*RandomNumberGenerator*/

void RandomNumberGenerator_initialise_instance(){
  RandomNumberGenerator::initialise_instance();
}

void RandomNumberGenerator_set_seed(long seed){
  RandomNumberGenerator::get_instance().set_seed(seed);
}

/*Model*/
CModel* CModel_new(
		   const CModelMethods* cbfuncs,
		   void* init_state
){ 
  return new CModel(cbfuncs,init_state);

}

void CModel_delete(CModel* self){
  delete self;
}

/*Sampler*/

//Constructor
CSampler* Sampler_new(const CModel* factory,double compression,const Options* options){
  return new Sampler<CModel>(*((CModel*)factory),compression,*((Options*)options));
}

void Sampler_delete(CSampler* self){
  delete self;
}

//Methods
void Sampler_loadLevels(CSampler* self,const char* filename){
  self->loadLevels(filename);
}

void Sampler_run(CSampler* self){
  self->run();
}    

/*Misc. utilities*/

ostream* create_ostream_handle_for_file(const char* filename){ //...need this for exposing DNest3::Model::print
  std::filebuf fb;
  fb.open (filename,std::ios::out);
  return new std::ostream(&fb);
}

void print_string_to_stream(ostream* out,const char* s){
  *out<<s;
}
