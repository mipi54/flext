/* 

flext - C++ layer for Max/MSP and pd (pure data) externals

Copyright (c) 2001,2002 Thomas Grill (xovo@gmx.net)
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "license.txt," in this distribution.  

*/

/*! \file flclass.h
	\brief User accessible flext base classes
    
*/

#ifndef __FLCLASS_H
#define __FLCLASS_H

// include the header file declaring the base classes
#include "flbase.h"
#include "flsupport.h"

#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#ifdef __BORLANDC__
#pragma warn -8008 // Condition is always false
#pragma warn -8057 // Parameter is never used
#pragma warn -8066 // Unreachable code
#endif


namespace flext {

// === flext_base ==================================================

//class qmsg;
//class thr_entry;

/*! \brief Flext message only base object
*/


class CBase: //flext_base:
	public flext_obj
{
	FLEXT_HEADER_S(flext_base,flext_obj,Setup)
	
public:

	/*!	\defgroup FLEXT_C_BASE Flext basic class functionality

		@{ 
	*/

// ---  compatibility mode ----------------------------------------

	/*! \brief Cross-platform compatibility flag.
		If set flext allows only operations valid for all platforms.
		Set to true by default!
	*/
	static bool compatibility;  

//!		@} 


// --- inheritable virtual methods --------------------------------

	/*!	\defgroup FLEXT_C_VIRTUAL Flext virtual base class functions

		@{ 
	*/

	//! called on "help" message: should post some text
	virtual void m_help();
	
	//! called on patcher load (not on mere object creation!)
	virtual void m_loadbang() {}

	//! quickhelp for inlets/outlets (gets calles in MaxMSP only)
	virtual void m_assist(long /*msg*/,long /*arg*/,char * /*s*/) {}

	/*!	\brief Called for every incoming message.
		All method handling is done in there
		\return True if a handler was found and called
	*/
	virtual bool m_methodmain(int inlet,const t_symbol *s,int argc,t_atom *argv);

	//! called for every unhandled message (by m_methodmain)
	virtual bool m_method_(int inlet,const t_symbol *s,int argc,t_atom *argv);

//!		@} 


// --- help -------------------------------------------------------	

	/*!	\defgroup FLEXT_C_HELP Flext help/assistance functionality

		@{ 
	*/

	void DefineHelp(const char *ref,const char *dir = NULL);

//!		@} 


// --- inlet/outlet stuff -----------------------------------------	

	/*!	\defgroup FLEXT_C_INOUT Flext in-/outlet functions
		\note These must be called in the class' constructor
		\note All (also default) inlets must be defined

		@{ 
	*/

	// argument m specifies multiple inlet/outlet count
	
//	void AddInDef() { AddInlet(xlet::tp_def,1); }

	/*! \brief Add inlet(s) for anythings
		That's the one to choose for the left-most (first) inlet unless it's a signal inlet.
	*/
	void AddInAnything(int m = 1) { AddInlet(xlet::tp_any,m); } // leftmost or via proxy
	void AddInAnything(const char *desc,int m = 1) { AddInlet(xlet::tp_any,m,desc); } // leftmost or via proxy
	//! Add inlet(s) for floats
	void AddInFloat(int m = 1) { AddInlet(xlet::tp_float,m); }
	void AddInFloat(const char *desc,int m = 1) { AddInlet(xlet::tp_float,m,desc); }
	//! Add inlet(s) for ints
	void AddInInt(int m = 1) { AddInlet(xlet::tp_int,m); }
	void AddInInt(const char *desc,int m = 1) { AddInlet(xlet::tp_int,m,desc); }
	//! Add inlet(s) for symbols
	void AddInSymbol(int m = 1) { AddInlet(xlet::tp_sym,m); }
	void AddInSymbol(const char *desc,int m = 1) { AddInlet(xlet::tp_sym,m,desc); }
	//! Add inlet(s) for bang
	void AddInBang(int m = 1) { AddInlet(xlet::tp_sym,m); }
	void AddInBang(const char *desc,int m = 1) { AddInlet(xlet::tp_sym,m,desc); }
	//! Add inlet(s) for lists
	void AddInList(int m = 1) { AddInlet(xlet::tp_list,m); }  // via proxy
	void AddInList(const char *desc,int m = 1) { AddInlet(xlet::tp_list,m,desc); }  // via proxy
	
	//! Add outlet(s) for anythings
	void AddOutAnything(int m = 1) { AddOutlet(xlet::tp_any,m); }
	void AddOutAnything(const char *desc,int m = 1) { AddOutlet(xlet::tp_any,m,desc); }
	//! Add outlet(s) for floats
	void AddOutFloat(int m = 1) { AddOutlet(xlet::tp_float,m); }
	void AddOutFloat(const char *desc,int m = 1) { AddOutlet(xlet::tp_float,m,desc); }
	//! Add outlet(s) for ints
	void AddOutInt(int m = 1) { AddOutlet(xlet::tp_int,m); }
	void AddOutInt(const char *desc,int m = 1) { AddOutlet(xlet::tp_int,m,desc); }
	//! Add outlet(s) for symbols
	void AddOutSymbol(int m = 1) { AddOutlet(xlet::tp_sym,m); }
	void AddOutSymbol(const char *desc,int m = 1) { AddOutlet(xlet::tp_sym,m,desc); }
	//! Add outlet(s) for bangs
	void AddOutBang(int m = 1) { AddOutlet(xlet::tp_sym,m); }
	void AddOutBang(const char *desc,int m = 1) { AddOutlet(xlet::tp_sym,m,desc); }
	//! Add outlet(s) for lists
	void AddOutList(int m = 1) { AddOutlet(xlet::tp_list,m); }
	void AddOutList(const char *desc,int m = 1) { AddOutlet(xlet::tp_list,m,desc); }
	
	/*! \brief Set up inlets and outlets
		\remark Must be called ONCE to actually set up the defined inlets/outlets.
		\return True on successful creation of all inlets and outlets
	*/
	bool SetupInOut(); 

	//! Get number of inlets
	int CntIn() const { return incnt; }
	//! Get number of outlets
	int CntOut() const { return outcnt; }
	//! Get number of signal inlets
	int CntInSig() const { return insigs; }
	//! Get number of signal outlets
	int CntOutSig() const { return outsigs; }


	class outlet;
//	class AtomList;
//	class AtomAnything;

	//! Get pointer to outlet (_after_ calling setup_inout()!)
	outlet *GetOut(int ix) { return (outlets && ix < outcnt)?outlets[ix]:NULL; }

	// output messages 

	void ToOutBang(outlet *o); 
	//! Output bang (index n starts with 0)
	void ToOutBang(int n) { outlet *o = GetOut(n); if(o) ToOutBang(o); }

	void ToOutFloat(outlet *o,float f); 
	//! Output float (index n starts with 0)
	void ToOutFloat(int n,float f) { outlet *o = GetOut(n); if(o) ToOutFloat(o,f); }

	void ToOutInt(outlet *o,int f); 
	//! Output int (index n starts with 0)
	void ToOutInt(int n,int f) { outlet *o = GetOut(n); if(o) ToOutInt(o,f); }
	
	void ToOutSymbol(outlet *o,const t_symbol *s); 
	//! Output symbol (index n starts with 0)
	void ToOutSymbol(int n,const t_symbol *s) { outlet *o = GetOut(n); if(o) ToOutSymbol(o,s); }

	void ToOutString(outlet *o,const char *s) { ToOutSymbol(o,gensym(const_cast<char *>(s))); }
	//! Output string (index n starts with 0)
	void ToOutString(int n,const char *s) { outlet *o = GetOut(n); if(o) ToOutString(o,s); }

	void ToOutList(outlet *o,int argc,const t_atom *argv); 
	//! Output list (index n starts with 0)
	void ToOutList(int n,int argc,const t_atom *argv)  { outlet *o = GetOut(n); if(o) ToOutList(o,argc,argv); }
	//! Output list (index n starts with 0)
	void ToOutList(int n,const AtomList &list)  { ToOutList(n,list.Count(),list.Atoms()); }
	
	void ToOutAnything(outlet *o,const t_symbol *s,int argc,const t_atom *argv); 
	//! Output anything (index n starts with 0)
	void ToOutAnything(int n,const t_symbol *s,int argc,const t_atom *argv)  { outlet *o = GetOut(n); if(o) ToOutAnything(o,const_cast<t_symbol *>(s),argc,argv); }
	//! Output anything (index n starts with 0)
	void ToOutAnything(int n,const AtomAnything &any)  { ToOutAnything(n,any.Header(),any.Count(),any.Atoms()); }
	
//!		@} 


// --- message handling -------------------------------------------

	enum metharg {
		a_null = 0,
		a_float,a_int, 
		a_symbol,a_pointer,
		a_gimme,a_xgimme
	};

	typedef bool (*methfun)(t_class *c);

	/*!	\defgroup FLEXT_C_ADDMETHOD Flext method handling
		\internal

		@{ 
	*/

	void AddMethodDef(int inlet); // call virtual function for inlet
	void AddMethodDef(int inlet,const char *tag); // call virtual function for tag && inlet
	void AddMethod(int inlet,const char *tag,methfun fun,metharg tp,...); 

	void AddMethod(int inlet,bool (*m)(flext_base *,int,t_atom *)) { AddMethod(inlet,"list",(methfun)m,a_gimme,a_null); }
	void AddMethod(int inlet,const char *tag,bool (*m)(flext_base *)) { AddMethod(inlet,tag,(methfun)m,a_null); }  // pure method
	void AddMethod(int inlet,bool (*m)(flext_base *,const t_symbol *,int,t_atom *)) { AddMethod(inlet,"anything",(methfun)m,a_xgimme,a_null); } // anything
	void AddMethod(int inlet,bool (*m)(flext_base *,t_symbol *&)) { AddMethod(inlet,"symbol",(methfun)m,a_symbol,a_null); } // single symbol
	void AddMethod(int inlet,bool (*m)(flext_base *,float &)) { AddMethod(inlet,"float",(methfun)m,a_float,a_null); }  // single float
	void AddMethod(int inlet,bool (*m)(flext_base *,float &,float &)) { AddMethod(inlet,"list",(methfun)m,a_float,a_float,a_null); } // list of 2 floats
	void AddMethod(int inlet,bool (*m)(flext_base *,float &,float &,float &)) { AddMethod(inlet,"list",(methfun)m,a_float,a_float,a_float,a_null); } // list of 3 floats
#ifdef PD
	void AddMethod(int inlet,bool (*m)(flext_base *,int &)) { AddMethod(inlet,"float",(methfun)m,a_int,a_null); }  // single float
#else
	void AddMethod(int inlet,bool (*m)(flext_base *,int &)) { AddMethod(inlet,"int",(methfun)m,a_int,a_null); }  // single float
#endif
	void AddMethod(int inlet,bool (*m)(flext_base *,int &,int &)) { AddMethod(inlet,"list",(methfun)m,a_int,a_int,a_null); } // list of 2 floats
	void AddMethod(int inlet,bool (*m)(flext_base *,int &,int &,int &)) { AddMethod(inlet,"list",(methfun)m,a_int,a_int,a_int,a_null); } // list of 3 floats
	void AddMethod(int inlet,const char *tag,bool (*m)(flext_base *,int,t_atom *)) { AddMethod(inlet,tag,(methfun)m,a_gimme,a_null); } // method+gimme
	void AddMethod(int inlet,const char *tag,bool (*m)(flext_base *,const t_symbol *,int,t_atom *)) { AddMethod(inlet,tag,(methfun)m,a_xgimme,a_null); } // method+gimme 
	void AddMethod(int inlet,const char *tag,bool (*m)(flext_base *,t_symbol *&)) { AddMethod(inlet,tag,(methfun)m,a_symbol,a_null); } // method+symbol
	void AddMethod(int inlet,const char *tag,bool (*m)(flext_base *,float &)) { AddMethod(inlet,tag,(methfun)m,a_float,a_null); }  // method+float
	void AddMethod(int inlet,const char *tag,bool (*m)(flext_base *,int &)) { AddMethod(inlet,tag,(methfun)m,a_int,a_null); } // method+int

	//! Set MaxMSP style of distributing list elements over (message) inlets
	void SetDist(bool d = true) { distmsgs = d; }

//!		@} 

// --- bind/unbind ---------------------------------------

#ifdef PD
	//! Bind object to a symbol
	bool Bind(const t_symbol *s) { pd_bind(&thisHdr()->ob_pd,const_cast<t_symbol *>(s)); return true; }
	//! Unbind object from a symbol
	bool Unbind(const t_symbol *s) { pd_unbind(&thisHdr()->ob_pd,const_cast<t_symbol *>(s)); return true; }
#else
	//! Bind object to a symbol
	bool Bind(const t_symbol *s) { if(s->s_thing) return false; else { const_cast<t_symbol *>(s)->s_thing = (t_object *)thisHdr(); return true; } }
	//! Unbind object from a symbol
	bool Unbind(const t_symbol *s) { if(s->s_thing != (t_object *)thisHdr()) return false; else { const_cast<t_symbol *>(s)->s_thing = NULL; return true; } }
#endif
	//! Bind object to a symbol (as string)
	bool Bind(const char *c) { return Bind(MakeSymbol(c)); }  
	//! Unbind object from a symbol (as string)
	bool Unbind(const char *c) { return Unbind(MakeSymbol(c)); }  

/*
	// Low level

	//! Bind object to a symbol
	static void DoUnbind(t_symbol *s,flext_obj *o);
	//! Unbind object from a symbol
	static void DoBind(const t_symbol *s,flext_obj *o);
	//! Get bound object of a symbol
	static t_class **GetBound(const t_symbol *s) { return (t_class **)s->s_thing; }
*/

// --- thread stuff -----------------------------------------------

#ifdef FLEXT_THREADS
	/*!	\defgroup FLEXT_C_THREAD Flext thread handling 

		@{ 
	*/

	/*! \brief Check if current thread should terminate
		\todo Check for currently running thread
	*/
	bool ShouldExit() const { return shouldexit; }

	//! Check if current thread is the realtime system's thread
	bool IsSystemThread() const { pthread_t cur = pthread_self(); return pthread_equal(cur,thrid) != 0; }

	/*! \brief Yield to other threads
		\remark A call to this is only needed for systems with cooperative multitasking like MacOS<=9
	*/
	static void ThrYield() { sched_yield(); }

	typedef pthread_t thrid_t;

	/*! \brief Increase/Decrease priority of a thread
	*/
	static bool ChangePriority(int dp,thrid_t thr = GetThreadId());

	/*! \brief Get priority of a thread
	*/
	static int GetPriority(thrid_t thr = GetThreadId());

	/*! \brief Set priority of a thread
	*/
	static bool SetPriority(int p,thrid_t thr = GetThreadId());

	/*! \brief Get current thread id
	*/
	static thrid_t GetThreadId();


#endif // FLEXT_THREADS

//!		@} 




// xxx internal stuff xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

// --- thread stuff -----------------------------------------------

#ifdef FLEXT_THREADS
	class thr_params;
	static bool StartThread(void *(*)(thr_params *p),thr_params *p,char *methname);
	bool PushThread();
	void PopThread();
#endif

protected:

//	flext_base();
//	virtual ~flext_base();

	CBase();
	virtual ~CBase();

// inlets and outlets
		
	struct xlet {	
		enum type {
			tp_none = 0,
			tp_float,tp_int,tp_sym,tp_list,tp_sig,tp_any
		};

		xlet(type t,const char *desc = NULL);
		~xlet();
		
		char *desc;
		type tp;
		xlet *nxt;
	};

	/*!	\addtogroup FLEXT_C_INOUT 

		@{ 
	*/

	unsigned long XletCode(xlet::type tp = xlet::tp_none,...); // end list with 0 (= tp_none) !!

	void AddInlets(unsigned long code); // use XletCode to get code value
	void AddInlet(xlet::type tp,int mult = 1,const char *desc = NULL) { AddXlet(tp,mult,desc,inlist); }
	void AddOutlets(unsigned long code); // use XletCode to get code value
	void AddOutlet(xlet::type tp,int mult = 1,const char *desc = NULL) { AddXlet(tp,mult,desc,outlist); }

	void DescInlet(int ix,const char *desc) { DescXlet(ix,desc,inlist); }
	void DescOutlet(int ix,const char *desc) { DescXlet(ix,desc,outlist); }

//!		@} 


#ifdef FLEXT_THREADS
	void QueueBang(outlet *o); 
	void QueueFloat(outlet *o,float f); 
	void QueueInt(outlet *o,int f); 
	void QueueSymbol(outlet *o,const t_symbol *s); 
	void QueueList(outlet *o,int argc,t_atom *argv); 
	void QueueAnything(outlet *o,const t_symbol *s,int argc,t_atom *argv); 
#endif

// method handling

	class methitem { 
	public:
		methitem(int inlet,t_symbol *t);
		~methitem();

		void SetArgs(methfun fun,int argc,metharg *args);

		t_symbol *tag;
		int inlet;
		int argc;
		metharg *args;
		methfun fun;
		
		methitem *nxt;
	};
	
	void AddMethItem(methitem *m);
	
private:

	static void Setup(t_class *c);

	xlet *inlist,*outlist;
	int incnt,outcnt,insigs,outsigs;
	outlet **outlets;
	bool distmsgs;

	void AddXlet(xlet::type tp,int mult,const char *desc,xlet *&root);	
	void DescXlet(int ix,const char *desc,xlet *&root);	

	methitem *mlst;

#ifdef FLEXT_THREADS
	bool shouldexit;
	int thrcount;
	
	pthread_t thrid;  // the thread that created the object (the system thread)

	class qmsg;
	qmsg *qhead,*qtail;
	t_qelem *qclk;
	ThrMutex qmutex;
#ifdef MAXMSP
	t_clock *yclk;
	static void YTick(flext_base *th);
#endif

	static void QTick(flext_base *th);
	void Queue(qmsg *m);

	class thr_entry;
	thr_entry *thrhead,*thrtail;
	ThrMutex tlmutex;
#endif

#ifdef PD
	// proxy object (for additional inlets) stuff
	struct px_object;
	friend struct px_object;
#elif defined(MAXMSP)
	typedef object px_object;
	static void cb_px_float(t_class *c,float f);
	static void cb_px_int(t_class *c,int v);
	static void cb_px_bang(t_class *c);

	static void cb_px_in1(t_class *c,int v);
	static void cb_px_in2(t_class *c,int v);
	static void cb_px_in3(t_class *c,int v);
	static void cb_px_in4(t_class *c,int v);
	static void cb_px_in5(t_class *c,int v);
	static void cb_px_in6(t_class *c,int v);
	static void cb_px_in7(t_class *c,int v);
	static void cb_px_in8(t_class *c,int v);
	static void cb_px_in9(t_class *c,int v);
#endif
	static void cb_px_anything(t_class *c,const t_symbol *s,int argc,t_atom *argv);

	static void cb_px_ft1(t_class *c,float f);
	static void cb_px_ft2(t_class *c,float f);
	static void cb_px_ft3(t_class *c,float f);
	static void cb_px_ft4(t_class *c,float f);
	static void cb_px_ft5(t_class *c,float f);
	static void cb_px_ft6(t_class *c,float f);
	static void cb_px_ft7(t_class *c,float f);
	static void cb_px_ft8(t_class *c,float f);
	static void cb_px_ft9(t_class *c,float f);

	px_object **inlets;

	// callback functions

	static void cb_help(t_class *c);

	static void cb_loadbang(t_class *c);
#ifdef MAXMSP
	static void cb_assist(t_class *c,void *b,long msg,long arg,char *s);
#endif
};

} // namespace flext

#endif
