/* Created by Language version: 7.7.0 */
/* VECTORIZED */
#define NRN_VECTORIZED 1
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mech_api.h"
#undef PI
#define nil 0
#include "md1redef.h"
#include "section.h"
#include "nrniv_mf.h"
#include "md2redef.h"
 
#if METHOD3
extern int _method3;
#endif

#if !NRNGPU
#undef exp
#define exp hoc_Exp
extern double hoc_Exp(double);
#endif
 
#define nrn_init _nrn_init__AMPANMDA_EMS
#define _nrn_initial _nrn_initial__AMPANMDA_EMS
#define nrn_cur _nrn_cur__AMPANMDA_EMS
#define _nrn_current _nrn_current__AMPANMDA_EMS
#define nrn_jacob _nrn_jacob__AMPANMDA_EMS
#define nrn_state _nrn_state__AMPANMDA_EMS
#define _net_receive _net_receive__AMPANMDA_EMS 
#define state state__AMPANMDA_EMS 
 
#define _threadargscomma_ _p, _ppvar, _thread, _nt,
#define _threadargsprotocomma_ double* _p, Datum* _ppvar, Datum* _thread, NrnThread* _nt,
#define _threadargs_ _p, _ppvar, _thread, _nt
#define _threadargsproto_ double* _p, Datum* _ppvar, Datum* _thread, NrnThread* _nt
 	/*SUPPRESS 761*/
	/*SUPPRESS 762*/
	/*SUPPRESS 763*/
	/*SUPPRESS 765*/
	 extern double *getarg();
 /* Thread safe. No static _p or _ppvar. */
 
#define t _nt->_t
#define dt _nt->_dt
#define tau_r_AMPA _p[0]
#define tau_r_AMPA_columnindex 0
#define tau_d_AMPA _p[1]
#define tau_d_AMPA_columnindex 1
#define tau_r_NMDA _p[2]
#define tau_r_NMDA_columnindex 2
#define tau_d_NMDA _p[3]
#define tau_d_NMDA_columnindex 3
#define e _p[4]
#define e_columnindex 4
#define gamma _p[5]
#define gamma_columnindex 5
#define NMDA_ratio _p[6]
#define NMDA_ratio_columnindex 6
#define i _p[7]
#define i_columnindex 7
#define i_AMPA _p[8]
#define i_AMPA_columnindex 8
#define i_NMDA _p[9]
#define i_NMDA_columnindex 9
#define g_AMPA _p[10]
#define g_AMPA_columnindex 10
#define g_NMDA _p[11]
#define g_NMDA_columnindex 11
#define g _p[12]
#define g_columnindex 12
#define A_AMPA_step _p[13]
#define A_AMPA_step_columnindex 13
#define B_AMPA_step _p[14]
#define B_AMPA_step_columnindex 14
#define A_NMDA_step _p[15]
#define A_NMDA_step_columnindex 15
#define B_NMDA_step _p[16]
#define B_NMDA_step_columnindex 16
#define A_AMPA _p[17]
#define A_AMPA_columnindex 17
#define B_AMPA _p[18]
#define B_AMPA_columnindex 18
#define A_NMDA _p[19]
#define A_NMDA_columnindex 19
#define B_NMDA _p[20]
#define B_NMDA_columnindex 20
#define factor_AMPA _p[21]
#define factor_AMPA_columnindex 21
#define factor_NMDA _p[22]
#define factor_NMDA_columnindex 22
#define DA_AMPA _p[23]
#define DA_AMPA_columnindex 23
#define DB_AMPA _p[24]
#define DB_AMPA_columnindex 24
#define DA_NMDA _p[25]
#define DA_NMDA_columnindex 25
#define DB_NMDA _p[26]
#define DB_NMDA_columnindex 26
#define v _p[27]
#define v_columnindex 27
#define _g _p[28]
#define _g_columnindex 28
#define _tsav _p[29]
#define _tsav_columnindex 29
#define _nd_area  *_ppvar[0]._pval
 
#if MAC
#if !defined(v)
#define v _mlhv
#endif
#if !defined(h)
#define h _mlhh
#endif
#endif
 
#if defined(__cplusplus)
extern "C" {
#endif
 static int hoc_nrnpointerindex =  -1;
 static Datum* _extcall_thread;
 static Prop* _extcall_prop;
 /* external NEURON variables */
 /* declaration of user functions */
 static double _hoc_state(void*);
 static int _mechtype;
extern void _nrn_cacheloop_reg(int, int);
extern void hoc_register_prop_size(int, int, int);
extern void hoc_register_limits(int, HocParmLimits*);
extern void hoc_register_units(int, HocParmUnits*);
extern void nrn_promote(Prop*, int, int);
extern Memb_func* memb_func;
 
#define NMODL_TEXT 1
#if NMODL_TEXT
static const char* nmodl_file_text;
static const char* nmodl_filename;
extern void hoc_reg_nmodl_text(int, const char*);
extern void hoc_reg_nmodl_filename(int, const char*);
#endif

 extern Prop* nrn_point_prop_;
 static int _pointtype;
 static void* _hoc_create_pnt(Object* _ho) { void* create_point_process(int, Object*);
 return create_point_process(_pointtype, _ho);
}
 static void _hoc_destroy_pnt(void*);
 static double _hoc_loc_pnt(void* _vptr) {double loc_point_process(int, void*);
 return loc_point_process(_pointtype, _vptr);
}
 static double _hoc_has_loc(void* _vptr) {double has_loc_point(void*);
 return has_loc_point(_vptr);
}
 static double _hoc_get_loc_pnt(void* _vptr) {
 double get_loc_point_process(void*); return (get_loc_point_process(_vptr));
}
 extern void _nrn_setdata_reg(int, void(*)(Prop*));
 static void _setdata(Prop* _prop) {
 _extcall_prop = _prop;
 }
 static void _hoc_setdata(void* _vptr) { Prop* _prop;
 _prop = ((Point_process*)_vptr)->_prop;
   _setdata(_prop);
 }
 /* connect user functions to hoc names */
 static VoidFunc hoc_intfunc[] = {
 0,0
};
 static Member_func _member_func[] = {
 "loc", _hoc_loc_pnt,
 "has_loc", _hoc_has_loc,
 "get_loc", _hoc_get_loc_pnt,
 "state", _hoc_state,
 0, 0
};
 /* declare global and static user variables */
 static int _thread1data_inuse = 0;
static double _thread1data[1];
#define _gth 0
#define mggate_AMPANMDA_EMS _thread1data[0]
#define mggate _thread[_gth]._pval[0]
#define mg mg_AMPANMDA_EMS
 double mg = 1;
 /* some parameters have upper and lower limits */
 static HocParmLimits _hoc_parm_limits[] = {
 0,0,0
};
 static HocParmUnits _hoc_parm_units[] = {
 "mg_AMPANMDA_EMS", "mM",
 "tau_r_AMPA", "ms",
 "tau_d_AMPA", "ms",
 "tau_r_NMDA", "ms",
 "tau_d_NMDA", "ms",
 "e", "mV",
 "gamma", "/mV",
 "NMDA_ratio", "1",
 "i", "nA",
 "i_AMPA", "nA",
 "i_NMDA", "nA",
 "g_AMPA", "uS",
 "g_NMDA", "uS",
 "g", "uS",
 0,0
};
 static double A_NMDA0 = 0;
 static double A_AMPA0 = 0;
 static double B_NMDA0 = 0;
 static double B_AMPA0 = 0;
 static double delta_t = 0.01;
 /* connect global user variables to hoc */
 static DoubScal hoc_scdoub[] = {
 "mg_AMPANMDA_EMS", &mg_AMPANMDA_EMS,
 "mggate_AMPANMDA_EMS", &mggate_AMPANMDA_EMS,
 0,0
};
 static DoubVec hoc_vdoub[] = {
 0,0,0
};
 static double _sav_indep;
 static void nrn_alloc(Prop*);
static void  nrn_init(NrnThread*, _Memb_list*, int);
static void nrn_state(NrnThread*, _Memb_list*, int);
 static void nrn_cur(NrnThread*, _Memb_list*, int);
static void  nrn_jacob(NrnThread*, _Memb_list*, int);
 static void _hoc_destroy_pnt(void* _vptr) {
   destroy_point_process(_vptr);
}
 
static int _ode_count(int);
 /* connect range variables in _p that hoc is supposed to know about */
 static const char *_mechanism[] = {
 "7.7.0",
"AMPANMDA_EMS",
 "tau_r_AMPA",
 "tau_d_AMPA",
 "tau_r_NMDA",
 "tau_d_NMDA",
 "e",
 "gamma",
 "NMDA_ratio",
 0,
 "i",
 "i_AMPA",
 "i_NMDA",
 "g_AMPA",
 "g_NMDA",
 "g",
 "A_AMPA_step",
 "B_AMPA_step",
 "A_NMDA_step",
 "B_NMDA_step",
 0,
 "A_AMPA",
 "B_AMPA",
 "A_NMDA",
 "B_NMDA",
 0,
 0};
 
extern Prop* need_memb(Symbol*);

static void nrn_alloc(Prop* _prop) {
	Prop *prop_ion;
	double *_p; Datum *_ppvar;
  if (nrn_point_prop_) {
	_prop->_alloc_seq = nrn_point_prop_->_alloc_seq;
	_p = nrn_point_prop_->param;
	_ppvar = nrn_point_prop_->dparam;
 }else{
 	_p = nrn_prop_data_alloc(_mechtype, 30, _prop);
 	/*initialize range parameters*/
 	tau_r_AMPA = 0.2;
 	tau_d_AMPA = 1.7;
 	tau_r_NMDA = 0.29;
 	tau_d_NMDA = 43;
 	e = 0;
 	gamma = 0.062;
 	NMDA_ratio = 0.71;
  }
 	_prop->param = _p;
 	_prop->param_size = 30;
  if (!nrn_point_prop_) {
 	_ppvar = nrn_prop_datum_alloc(_mechtype, 2, _prop);
  }
 	_prop->dparam = _ppvar;
 	/*connect ionic variables to this model*/
 
}
 static void _initlists();
 static void _net_receive(Point_process*, double*, double);
 static void _thread_mem_init(Datum*);
 static void _thread_cleanup(Datum*);
 extern Symbol* hoc_lookup(const char*);
extern void _nrn_thread_reg(int, int, void(*)(Datum*));
extern void _nrn_thread_table_reg(int, void(*)(double*, Datum*, Datum*, NrnThread*, int));
extern void hoc_register_tolerance(int, HocStateTolerance*, Symbol***);
extern void _cvode_abstol( Symbol**, double*, int);

 void _AMPANMDA_EMS_reg() {
	int _vectorized = 1;
  _initlists();
 	_pointtype = point_register_mech(_mechanism,
	 nrn_alloc,nrn_cur, nrn_jacob, nrn_state, nrn_init,
	 hoc_nrnpointerindex, 2,
	 _hoc_create_pnt, _hoc_destroy_pnt, _member_func);
  _extcall_thread = (Datum*)ecalloc(1, sizeof(Datum));
  _thread_mem_init(_extcall_thread);
  _thread1data_inuse = 0;
 _mechtype = nrn_get_mechtype(_mechanism[1]);
     _nrn_setdata_reg(_mechtype, _setdata);
     _nrn_thread_reg(_mechtype, 1, _thread_mem_init);
     _nrn_thread_reg(_mechtype, 0, _thread_cleanup);
 #if NMODL_TEXT
  hoc_reg_nmodl_text(_mechtype, nmodl_file_text);
  hoc_reg_nmodl_filename(_mechtype, nmodl_filename);
#endif
  hoc_register_prop_size(_mechtype, 30, 2);
  hoc_register_dparam_semantics(_mechtype, 0, "area");
  hoc_register_dparam_semantics(_mechtype, 1, "pntproc");
 	hoc_register_cvode(_mechtype, _ode_count, 0, 0, 0);
 pnt_receive[_mechtype] = _net_receive;
 pnt_receive_size[_mechtype] = 3;
 	hoc_register_var(hoc_scdoub, hoc_vdoub, hoc_intfunc);
 	ivoc_help("help ?1 AMPANMDA_EMS AMPANMDA_EMS.mod\n");
 hoc_register_limits(_mechtype, _hoc_parm_limits);
 hoc_register_units(_mechtype, _hoc_parm_units);
 }
static int _reset;
static char *modelname = "Probabilistic AMPA and NMDA receptor with presynaptic short-term plasticity ";

static int error;
static int _ninits = 0;
static int _match_recurse=1;
static void _modl_cleanup(){ _match_recurse=1;}
static int state(_threadargsproto_);
 
static int  state ( _threadargsproto_ ) {
   A_AMPA = A_AMPA * A_AMPA_step ;
   B_AMPA = B_AMPA * B_AMPA_step ;
   A_NMDA = A_NMDA * A_NMDA_step ;
   B_NMDA = B_NMDA * B_NMDA_step ;
    return 0; }
 
static double _hoc_state(void* _vptr) {
 double _r;
   double* _p; Datum* _ppvar; Datum* _thread; NrnThread* _nt;
   _p = ((Point_process*)_vptr)->_prop->param;
  _ppvar = ((Point_process*)_vptr)->_prop->dparam;
  _thread = _extcall_thread;
  _nt = (NrnThread*)((Point_process*)_vptr)->_vnt;
 _r = 1.;
 state ( _p, _ppvar, _thread, _nt );
 return(_r);
}
 
static void _net_receive (Point_process* _pnt, double* _args, double _lflag) 
{  double* _p; Datum* _ppvar; Datum* _thread; NrnThread* _nt;
   _thread = (Datum*)0; _nt = (NrnThread*)_pnt->_vnt;   _p = _pnt->_prop->param; _ppvar = _pnt->_prop->dparam;
  if (_tsav > t){ extern char* hoc_object_name(); hoc_execerror(hoc_object_name(_pnt->ob), ":Event arrived out of order. Must call ParallelContext.set_maxstep AFTER assigning minimum NetCon.delay");}
 _tsav = t; {
   _args[1] = _args[0] ;
   _args[2] = _args[0] * NMDA_ratio ;
   A_AMPA = A_AMPA + _args[1] * factor_AMPA ;
   B_AMPA = B_AMPA + _args[1] * factor_AMPA ;
   A_NMDA = A_NMDA + _args[2] * factor_NMDA ;
   B_NMDA = B_NMDA + _args[2] * factor_NMDA ;
   } }
 
static int _ode_count(int _type){ hoc_execerror("AMPANMDA_EMS", "cannot be used with CVODE"); return 0;}
 
static void _thread_mem_init(Datum* _thread) {
  if (_thread1data_inuse) {_thread[_gth]._pval = (double*)ecalloc(1, sizeof(double));
 }else{
 _thread[_gth]._pval = _thread1data; _thread1data_inuse = 1;
 }
 }
 
static void _thread_cleanup(Datum* _thread) {
  if (_thread[_gth]._pval == _thread1data) {
   _thread1data_inuse = 0;
  }else{
   free((void*)_thread[_gth]._pval);
  }
 }

static void initmodel(double* _p, Datum* _ppvar, Datum* _thread, NrnThread* _nt) {
  int _i; double _save;{
  A_NMDA = A_NMDA0;
  A_AMPA = A_AMPA0;
  B_NMDA = B_NMDA0;
  B_AMPA = B_AMPA0;
 {
   double _ltp_AMPA , _ltp_NMDA ;
 A_AMPA = 0.0 ;
   B_AMPA = 0.0 ;
   A_NMDA = 0.0 ;
   B_NMDA = 0.0 ;
   _ltp_AMPA = ( tau_r_AMPA * tau_d_AMPA ) / ( tau_d_AMPA - tau_r_AMPA ) * log ( tau_d_AMPA / tau_r_AMPA ) ;
   _ltp_NMDA = ( tau_r_NMDA * tau_d_NMDA ) / ( tau_d_NMDA - tau_r_NMDA ) * log ( tau_d_NMDA / tau_r_NMDA ) ;
   factor_AMPA = - exp ( - _ltp_AMPA / tau_r_AMPA ) + exp ( - _ltp_AMPA / tau_d_AMPA ) ;
   factor_AMPA = 1.0 / factor_AMPA ;
   factor_NMDA = - exp ( - _ltp_NMDA / tau_r_NMDA ) + exp ( - _ltp_NMDA / tau_d_NMDA ) ;
   factor_NMDA = 1.0 / factor_NMDA ;
   A_AMPA_step = exp ( dt * ( ( - 1.0 ) / tau_r_AMPA ) ) ;
   B_AMPA_step = exp ( dt * ( ( - 1.0 ) / tau_d_AMPA ) ) ;
   A_NMDA_step = exp ( dt * ( ( - 1.0 ) / tau_r_NMDA ) ) ;
   B_NMDA_step = exp ( dt * ( ( - 1.0 ) / tau_d_NMDA ) ) ;
   }
 
}
}

static void nrn_init(NrnThread* _nt, _Memb_list* _ml, int _type){
double* _p; Datum* _ppvar; Datum* _thread;
Node *_nd; double _v; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
_thread = _ml->_thread;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
 _tsav = -1e20;
#if CACHEVEC
  if (use_cachevec) {
    _v = VEC_V(_ni[_iml]);
  }else
#endif
  {
    _nd = _ml->_nodelist[_iml];
    _v = NODEV(_nd);
  }
 v = _v;
 initmodel(_p, _ppvar, _thread, _nt);
}
}

static double _nrn_current(double* _p, Datum* _ppvar, Datum* _thread, NrnThread* _nt, double _v){double _current=0.;v=_v;{ {
   mggate = 1.0 / ( 1.0 + exp ( gamma * - ( v ) ) * ( mg / 3.57 ) ) ;
   g_AMPA = B_AMPA - A_AMPA ;
   g_NMDA = ( B_NMDA - A_NMDA ) * mggate ;
   g = g_AMPA + g_NMDA ;
   i_AMPA = g_AMPA * ( v - e ) ;
   i_NMDA = g_NMDA * ( v - e ) ;
   i = i_AMPA + i_NMDA ;
   }
 _current += i;

} return _current;
}

static void nrn_cur(NrnThread* _nt, _Memb_list* _ml, int _type) {
double* _p; Datum* _ppvar; Datum* _thread;
Node *_nd; int* _ni; double _rhs, _v; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
_thread = _ml->_thread;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
#if CACHEVEC
  if (use_cachevec) {
    _v = VEC_V(_ni[_iml]);
  }else
#endif
  {
    _nd = _ml->_nodelist[_iml];
    _v = NODEV(_nd);
  }
 _g = _nrn_current(_p, _ppvar, _thread, _nt, _v + .001);
 	{ _rhs = _nrn_current(_p, _ppvar, _thread, _nt, _v);
 	}
 _g = (_g - _rhs)/.001;
 _g *=  1.e2/(_nd_area);
 _rhs *= 1.e2/(_nd_area);
#if CACHEVEC
  if (use_cachevec) {
	VEC_RHS(_ni[_iml]) -= _rhs;
  }else
#endif
  {
	NODERHS(_nd) -= _rhs;
  }
 
}
 
}

static void nrn_jacob(NrnThread* _nt, _Memb_list* _ml, int _type) {
double* _p; Datum* _ppvar; Datum* _thread;
Node *_nd; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
_thread = _ml->_thread;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml];
#if CACHEVEC
  if (use_cachevec) {
	VEC_D(_ni[_iml]) += _g;
  }else
#endif
  {
     _nd = _ml->_nodelist[_iml];
	NODED(_nd) += _g;
  }
 
}
 
}

static void nrn_state(NrnThread* _nt, _Memb_list* _ml, int _type) {
double* _p; Datum* _ppvar; Datum* _thread;
Node *_nd; double _v = 0.0; int* _ni; int _iml, _cntml;
#if CACHEVEC
    _ni = _ml->_nodeindices;
#endif
_cntml = _ml->_nodecount;
_thread = _ml->_thread;
for (_iml = 0; _iml < _cntml; ++_iml) {
 _p = _ml->_data[_iml]; _ppvar = _ml->_pdata[_iml];
 _nd = _ml->_nodelist[_iml];
#if CACHEVEC
  if (use_cachevec) {
    _v = VEC_V(_ni[_iml]);
  }else
#endif
  {
    _nd = _ml->_nodelist[_iml];
    _v = NODEV(_nd);
  }
 v=_v;
{
 {  { state(_p, _ppvar, _thread, _nt); }
  }}}

}

static void terminal(){}

static void _initlists(){
 double _x; double* _p = &_x;
 int _i; static int _first = 1;
  if (!_first) return;
_first = 0;
}

#if defined(__cplusplus)
} /* extern "C" */
#endif

#if NMODL_TEXT
static const char* nmodl_filename = "AMPANMDA_EMS.mod";
static const char* nmodl_file_text = 
  "COMMENT\n"
  "/*                                                                               \n"
  "Copyright (c) 2015 EPFL-BBP, All rights reserved.                                \n"
  "                                                                                 \n"
  "THIS SOFTWARE IS PROVIDED BY THE BLUE BRAIN PROJECT ``AS IS''                    \n"
  "AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,            \n"
  "THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR           \n"
  "PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE BLUE BRAIN PROJECT                 \n"
  "BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR           \n"
  "CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF             \n"
  "SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR                  \n"
  "BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,            \n"
  "WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE             \n"
  "OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN           \n"
  "IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                    \n"
  "                                                                                 \n"
  "This work is licensed under a                                                    \n"
  "Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License. \n"
  "To view a copy of this license, visit                                            \n"
  "http://creativecommons.org/licenses/by-nc-sa/4.0/legalcode or send a letter to   \n"
  "Creative Commons,                                                                \n"
  "171 Second Street, Suite 300,                                                    \n"
  "San Francisco, California, 94105, USA.                                           \n"
  "*/                 \n"
  "ENDCOMMENT\n"
  "\n"
  "TITLE Probabilistic AMPA and NMDA receptor with presynaptic short-term plasticity \n"
  "\n"
  "\n"
  "COMMENT\n"
  "AMPA and NMDA receptor conductance using a dual-exponential profile\n"
  "presynaptic short-term plasticity as in Fuhrmann et al. 2002\n"
  "\n"
  "_EMS (Eilif Michael Srikanth)\n"
  "Modification of ProbAMPANMDA: 2-State model by Eilif Muller, Michael Reimann, Srikanth Ramaswamy, Blue Brain Project, August 2011\n"
  "This new model was motivated by the following constraints:\n"
  "\n"
  "1) No consumption on failure.  \n"
  "2) No release just after release until recovery.\n"
  "3) Same ensemble averaged trace as deterministic/canonical Tsodyks-Markram \n"
  "   using same parameters determined from experiment.\n"
  "4) Same quantal size as present production probabilistic model.\n"
  "\n"
  "To satisfy these constaints, the synapse is implemented as a\n"
  "uni-vesicular (generalization to multi-vesicular should be\n"
  "straight-forward) 2-state Markov process.  The states are\n"
  "{1=recovered, 0=unrecovered}.\n"
  "\n"
  "For a pre-synaptic spike or external spontaneous release trigger\n"
  "event, the synapse will only release if it is in the recovered state,\n"
  "and with probability u (which follows facilitation dynamics).  If it\n"
  "releases, it will transition to the unrecovered state.  Recovery is as\n"
  "a Poisson process with rate 1/Dep.\n"
  "\n"
  "This model satisfies all of (1)-(4).\n"
  "ENDCOMMENT\n"
  "\n"
  "COMMENT                                                                          \n"
  "/**                                                                              \n"
  " @file ProbAMPANMDA_EMS.mod                                                        \n"
  " @brief Probabilistic AMPA and NMDA receptor with presynaptic short-term plasticity                   \n"
  " @author Eilif Muller, Michael Reimann, Srikanth Ramaswamy, James King @ BBP     \n"
  " @date 2011                                                                      \n"
  "*/                                                                               \n"
  "ENDCOMMENT  \n"
  "\n"
  "NEURON {\n"
  "    THREADSAFE\n"
  "        POINT_PROCESS AMPANMDA_EMS\n"
  "        RANGE tau_r_AMPA, tau_d_AMPA, tau_r_NMDA, tau_d_NMDA\n"
  "        RANGE i, i_AMPA, i_NMDA, g_AMPA, g_NMDA, g, e, NMDA_ratio\n"
  "        RANGE A_AMPA_step, B_AMPA_step, A_NMDA_step, B_NMDA_step\n"
  "        RANGE gamma\n"
  "        NONSPECIFIC_CURRENT i\n"
  "}\n"
  "\n"
  "PARAMETER {\n"
  "\n"
  "\n"
  "        tau_r_AMPA = 0.2   (ms)  : dual-exponential conductance profile\n"
  "        tau_d_AMPA = 1.7    (ms)  : IMPORTANT: tau_r < tau_d\n"
  "        tau_r_NMDA = 0.29   (ms) : dual-exponential conductance profile\n"
  "        tau_d_NMDA = 43     (ms) : IMPORTANT: tau_r < tau_d\n"
  "\n"
  "        e = 0     (mV)  : AMPA and NMDA reversal potential\n"
  "        mg = 1   (mM)  : initial concentration of mg2+\n"
  "        mggate\n"
  "        gamma = 0.062 (/mV)\n"
  "        \n"
  "	   NMDA_ratio = 0.71 (1) : The ratio of NMDA to AMPA\n"
  "}\n"
  "\n"
  "COMMENT\n"
  "The Verbatim block is needed to generate random nos. from a uniform distribution between 0 and 1 \n"
  "for comparison with Pr to decide whether to activate the synapse or not\n"
  "ENDCOMMENT\n"
  "\n"
  "\n"
  "ASSIGNED {\n"
  "\n"
  "        v (mV)\n"
  "        i (nA)\n"
  "        i_AMPA (nA)\n"
  "        i_NMDA (nA)\n"
  "        g_AMPA (uS)\n"
  "        g_NMDA (uS)\n"
  "        g (uS)\n"
  "        factor_AMPA\n"
  "        factor_NMDA\n"
  "        A_AMPA_step\n"
  "        B_AMPA_step\n"
  "        A_NMDA_step\n"
  "        B_NMDA_step\n"
  "        \n"
  "\n"
  "	: Recording these three, you can observe full state of model\n"
  "	: tsyn_fac gives you presynaptic times, Rstate gives you \n"
  "        : state transitions,\n"
  "        : u gives you the \"release probability\" at transitions \n"
  "        : (attention: u is event based based, so only valid at incoming events)\n"
  "	\n"
  "\n"
  "}\n"
  "\n"
  "STATE {\n"
  "\n"
  "        A_AMPA       : AMPA state variable to construct the dual-exponential profile - decays with conductance tau_r_AMPA\n"
  "        B_AMPA       : AMPA state variable to construct the dual-exponential profile - decays with conductance tau_d_AMPA\n"
  "        A_NMDA       : NMDA state variable to construct the dual-exponential profile - decays with conductance tau_r_NMDA\n"
  "        B_NMDA       : NMDA state variable to construct the dual-exponential profile - decays with conductance tau_d_NMDA\n"
  "}\n"
  "\n"
  "INITIAL{\n"
  "\n"
  "        LOCAL tp_AMPA, tp_NMDA\n"
  "\n"
  "\n"
  "        A_AMPA = 0\n"
  "        B_AMPA = 0\n"
  "        \n"
  "        A_NMDA = 0\n"
  "        B_NMDA = 0\n"
  "        \n"
  "        tp_AMPA = (tau_r_AMPA*tau_d_AMPA)/(tau_d_AMPA-tau_r_AMPA)*log(tau_d_AMPA/tau_r_AMPA) :time to peak of the conductance\n"
  "        tp_NMDA = (tau_r_NMDA*tau_d_NMDA)/(tau_d_NMDA-tau_r_NMDA)*log(tau_d_NMDA/tau_r_NMDA) :time to peak of the conductance\n"
  "        \n"
  "        factor_AMPA = -exp(-tp_AMPA/tau_r_AMPA)+exp(-tp_AMPA/tau_d_AMPA) :AMPA Normalization factor - so that when t = tp_AMPA, gsyn = gpeak\n"
  "        factor_AMPA = 1/factor_AMPA\n"
  "        \n"
  "        factor_NMDA = -exp(-tp_NMDA/tau_r_NMDA)+exp(-tp_NMDA/tau_d_NMDA) :NMDA Normalization factor - so that when t = tp_NMDA, gsyn = gpeak\n"
  "        factor_NMDA = 1/factor_NMDA\n"
  "\n"
  "        A_AMPA_step = exp(dt*(( - 1.0 ) / tau_r_AMPA))\n"
  "        B_AMPA_step = exp(dt*(( - 1.0 ) / tau_d_AMPA))\n"
  "        A_NMDA_step = exp(dt*(( - 1.0 ) / tau_r_NMDA))\n"
  "        B_NMDA_step = exp(dt*(( - 1.0 ) / tau_d_NMDA))\n"
  "}\n"
  "\n"
  "BREAKPOINT {\n"
  "\n"
  "        SOLVE state\n"
  "        mggate = 1 / (1 + exp(gamma  * -(v)) * (mg / 3.57 (mM))) :mggate kinetics - Jahr & Stevens 1990\n"
  "        g_AMPA = B_AMPA-A_AMPA :compute time varying conductance as the difference of state variables B_AMPA and A_AMPA\n"
  "        g_NMDA = (B_NMDA-A_NMDA) * mggate :compute time varying conductance as the difference of state variables B_NMDA and A_NMDA and mggate kinetics\n"
  "        g = g_AMPA + g_NMDA\n"
  "        i_AMPA = g_AMPA*(v-e) :compute the AMPA driving force based on the time varying conductance, membrane potential, and AMPA reversal\n"
  "        i_NMDA = g_NMDA*(v-e) :compute the NMDA driving force based on the time varying conductance, membrane potential, and NMDA reversal\n"
  "        i = i_AMPA + i_NMDA\n"
  "}\n"
  "\n"
  "PROCEDURE state() {\n"
  "        A_AMPA = A_AMPA*A_AMPA_step\n"
  "        B_AMPA = B_AMPA*B_AMPA_step\n"
  "        A_NMDA = A_NMDA*A_NMDA_step\n"
  "        B_NMDA = B_NMDA*B_NMDA_step\n"
  "}\n"
  "\n"
  "\n"
  "NET_RECEIVE (weight,weight_AMPA, weight_NMDA){\n"
  "        weight_AMPA = weight\n"
  "        weight_NMDA = weight * NMDA_ratio\n"
  "         A_AMPA = A_AMPA + weight_AMPA*factor_AMPA\n"
  "         B_AMPA = B_AMPA + weight_AMPA*factor_AMPA\n"
  "         A_NMDA = A_NMDA + weight_NMDA*factor_NMDA\n"
  "         B_NMDA = B_NMDA + weight_NMDA*factor_NMDA\n"
  "}\n"
  ;
#endif
