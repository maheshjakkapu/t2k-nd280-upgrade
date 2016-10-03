//
// File generated by /home/evrazhka/ND280/nd280rep/v11r31/ROOT/v5r34p09n04/amd64_linux26/bin/rootcint at Tue Aug 23 15:23:57 2016

// Do NOT change. Changes will be lost next time file is generated
//

#define R__DICTIONARY_FILENAME dOdOdIdictdIVersionManager_Dict
#include "RConfig.h" //rootcint 4834
#if !defined(R__ACCESS_IN_SYMBOL)
//Break the privacy of classes -- Disabled for the moment
#define private public
#define protected public
#endif

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;
#include "VersionManager_Dict.h"

#include "TClass.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"

// START OF SHADOWS

namespace ROOT {
   namespace Shadow {
   } // of namespace Shadow
} // of namespace ROOT
// END OF SHADOWS

namespace ROOT {
   void PackageVersion_ShowMembers(void *obj, TMemberInspector &R__insp);
   static void *new_PackageVersion(void *p = 0);
   static void *newArray_PackageVersion(Long_t size, void *p);
   static void delete_PackageVersion(void *p);
   static void deleteArray_PackageVersion(void *p);
   static void destruct_PackageVersion(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::PackageVersion*)
   {
      ::PackageVersion *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::PackageVersion >(0);
      static ::ROOT::TGenericClassInfo 
         instance("PackageVersion", ::PackageVersion::Class_Version(), "./../src/VersionManager.hxx", 16,
                  typeid(::PackageVersion), DefineBehavior(ptr, ptr),
                  &::PackageVersion::Dictionary, isa_proxy, 4,
                  sizeof(::PackageVersion) );
      instance.SetNew(&new_PackageVersion);
      instance.SetNewArray(&newArray_PackageVersion);
      instance.SetDelete(&delete_PackageVersion);
      instance.SetDeleteArray(&deleteArray_PackageVersion);
      instance.SetDestructor(&destruct_PackageVersion);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::PackageVersion*)
   {
      return GenerateInitInstanceLocal((::PackageVersion*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::PackageVersion*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
TClass *PackageVersion::fgIsA = 0;  // static to hold class pointer

//______________________________________________________________________________
const char *PackageVersion::Class_Name()
{
   return "PackageVersion";
}

//______________________________________________________________________________
const char *PackageVersion::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PackageVersion*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int PackageVersion::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::PackageVersion*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
void PackageVersion::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PackageVersion*)0x0)->GetClass();
}

//______________________________________________________________________________
TClass *PackageVersion::Class()
{
   if (!fgIsA) fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::PackageVersion*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
void PackageVersion::Streamer(TBuffer &R__b)
{
   // Stream an object of class PackageVersion.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(PackageVersion::Class(),this);
   } else {
      R__b.WriteClassBuffer(PackageVersion::Class(),this);
   }
}

//______________________________________________________________________________
void PackageVersion::ShowMembers(TMemberInspector &R__insp)
{
      // Inspect the data members of an object of class PackageVersion.
      TClass *R__cl = ::PackageVersion::IsA();
      if (R__cl || R__insp.IsA()) { }
      R__insp.Inspect(R__cl, R__insp.GetParent(), "_name", (void*)&_name);
      R__insp.InspectMember("string", (void*)&_name, "_name.", false);
      R__insp.Inspect(R__cl, R__insp.GetParent(), "_version", (void*)&_version);
      R__insp.InspectMember("string", (void*)&_version, "_version.", false);
      TObject::ShowMembers(R__insp);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_PackageVersion(void *p) {
      return  p ? new(p) ::PackageVersion : new ::PackageVersion;
   }
   static void *newArray_PackageVersion(Long_t nElements, void *p) {
      return p ? new(p) ::PackageVersion[nElements] : new ::PackageVersion[nElements];
   }
   // Wrapper around operator delete
   static void delete_PackageVersion(void *p) {
      delete ((::PackageVersion*)p);
   }
   static void deleteArray_PackageVersion(void *p) {
      delete [] ((::PackageVersion*)p);
   }
   static void destruct_PackageVersion(void *p) {
      typedef ::PackageVersion current_t;
      ((current_t*)p)->~current_t();
   }
} // end of namespace ROOT for class ::PackageVersion

/********************************************************
* ../dict/VersionManager_Dict.cxx
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************/

#ifdef G__MEMTEST
#undef malloc
#undef free
#endif

#if defined(__GNUC__) && __GNUC__ >= 4 && ((__GNUC_MINOR__ == 2 && __GNUC_PATCHLEVEL__ >= 1) || (__GNUC_MINOR__ >= 3))
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

extern "C" void G__cpp_reset_tagtableVersionManager_Dict();

extern "C" void G__set_cpp_environmentVersionManager_Dict() {
  G__cpp_reset_tagtableVersionManager_Dict();
}
#include <new>
extern "C" int G__cpp_dllrevVersionManager_Dict() { return(30051515); }

/*********************************************************
* Member function Interface Method
*********************************************************/

/* PackageVersion */
static int G__VersionManager_Dict_496_0_1(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   PackageVersion* p = NULL;
   char* gvp = (char*) G__getgvp();
   int n = G__getaryconstruct();
   if (n) {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new PackageVersion[n];
     } else {
       p = new((void*) gvp) PackageVersion[n];
     }
   } else {
     if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
       p = new PackageVersion;
     } else {
       p = new((void*) gvp) PackageVersion;
     }
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__VersionManager_DictLN_PackageVersion));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__VersionManager_Dict_496_0_2(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   PackageVersion* p = NULL;
   char* gvp = (char*) G__getgvp();
   //m: 2
   if ((gvp == (char*)G__PVOID) || (gvp == 0)) {
     p = new PackageVersion(*(string*) libp->para[0].ref, *(string*) libp->para[1].ref);
   } else {
     p = new((void*) gvp) PackageVersion(*(string*) libp->para[0].ref, *(string*) libp->para[1].ref);
   }
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__VersionManager_DictLN_PackageVersion));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__VersionManager_Dict_496_0_4(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      {
         const string& obj = ((const PackageVersion*) G__getstructoffset())->Name();
         result7->ref = (long) (&obj);
         result7->obj.i = (long) (&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__VersionManager_Dict_496_0_5(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((PackageVersion*) G__getstructoffset())->SetName(*(string*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__VersionManager_Dict_496_0_6(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      {
         const string& obj = ((const PackageVersion*) G__getstructoffset())->Version();
         result7->ref = (long) (&obj);
         result7->obj.i = (long) (&obj);
      }
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__VersionManager_Dict_496_0_7(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((PackageVersion*) G__getstructoffset())->SetVersion(*(string*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__VersionManager_Dict_496_0_8(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 85, (long) PackageVersion::Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__VersionManager_Dict_496_0_9(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) PackageVersion::Class_Name());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__VersionManager_Dict_496_0_10(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 115, (long) PackageVersion::Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__VersionManager_Dict_496_0_11(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      PackageVersion::Dictionary();
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__VersionManager_Dict_496_0_15(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      ((PackageVersion*) G__getstructoffset())->StreamerNVirtual(*(TBuffer*) libp->para[0].ref);
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__VersionManager_Dict_496_0_16(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) PackageVersion::DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__VersionManager_Dict_496_0_17(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) PackageVersion::ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__VersionManager_Dict_496_0_18(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 67, (long) PackageVersion::ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__VersionManager_Dict_496_0_19(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
      G__letint(result7, 105, (long) PackageVersion::DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__VersionManager_Dict_496_0_20(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)

{
   PackageVersion* p;
   void* tmp = (void*) G__int(libp->para[0]);
   p = new PackageVersion(*(PackageVersion*) tmp);
   result7->obj.i = (long) p;
   result7->ref = (long) p;
   G__set_tagnum(result7,G__get_linked_tagnum(&G__VersionManager_DictLN_PackageVersion));
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
typedef PackageVersion G__TPackageVersion;
static int G__VersionManager_Dict_496_0_21(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   char* gvp = (char*) G__getgvp();
   long soff = G__getstructoffset();
   int n = G__getaryconstruct();
   //
   //has_a_delete: 1
   //has_own_delete1arg: 0
   //has_own_delete2arg: 0
   //
   if (!soff) {
     return(1);
   }
   if (n) {
     if (gvp == (char*)G__PVOID) {
       delete[] (PackageVersion*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       for (int i = n - 1; i >= 0; --i) {
         ((PackageVersion*) (soff+(sizeof(PackageVersion)*i)))->~G__TPackageVersion();
       }
       G__setgvp((long)gvp);
     }
   } else {
     if (gvp == (char*)G__PVOID) {
       delete (PackageVersion*) soff;
     } else {
       G__setgvp((long) G__PVOID);
       ((PackageVersion*) (soff))->~G__TPackageVersion();
       G__setgvp((long)gvp);
     }
   }
   G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic assignment operator
static int G__VersionManager_Dict_496_0_22(G__value* result7, G__CONST char* funcname, struct G__param* libp, int hash)
{
   PackageVersion* dest = (PackageVersion*) G__getstructoffset();
   *dest = *(PackageVersion*) libp->para[0].ref;
   const PackageVersion& obj = *dest;
   result7->ref = (long) (&obj);
   result7->obj.i = (long) (&obj);
   return(1 || funcname || hash || result7 || libp) ;
}


/* Setting up global function */

/*********************************************************
* Member function Stub
*********************************************************/

/* PackageVersion */

/*********************************************************
* Global function Stub
*********************************************************/

/*********************************************************
* Get size of pointer to member function
*********************************************************/
class G__Sizep2memfuncVersionManager_Dict {
 public:
  G__Sizep2memfuncVersionManager_Dict(): p(&G__Sizep2memfuncVersionManager_Dict::sizep2memfunc) {}
    size_t sizep2memfunc() { return(sizeof(p)); }
  private:
    size_t (G__Sizep2memfuncVersionManager_Dict::*p)();
};

size_t G__get_sizep2memfuncVersionManager_Dict()
{
  G__Sizep2memfuncVersionManager_Dict a;
  G__setsizep2memfunc((int)a.sizep2memfunc());
  return((size_t)a.sizep2memfunc());
}


/*********************************************************
* virtual base class offset calculation interface
*********************************************************/

   /* Setting up class inheritance */

/*********************************************************
* Inheritance information setup/
*********************************************************/
extern "C" void G__cpp_setup_inheritanceVersionManager_Dict() {

   /* Setting up class inheritance */
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__VersionManager_DictLN_PackageVersion))) {
     PackageVersion *G__Lderived;
     G__Lderived=(PackageVersion*)0x1000;
     {
       TObject *G__Lpbase=(TObject*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__VersionManager_DictLN_PackageVersion),G__get_linked_tagnum(&G__VersionManager_DictLN_TObject),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
}

/*********************************************************
* typedef information setup/
*********************************************************/
extern "C" void G__cpp_setup_typetableVersionManager_Dict() {

   /* Setting up typedef entry */
   G__search_typename2("Version_t",115,-1,0,-1);
   G__setnewtype(-1,"Class version identifier (short)",0);
   G__search_typename2("vector<ROOT::TSchemaHelper>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<TVirtualArray*>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<std::bidirectional_iterator_tag,TObject*,std::ptrdiff_t,const TObject**,const TObject*&>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<bidirectional_iterator_tag,TObject*,std::ptrdiff_t,const TObject**,const TObject*&>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<bidirectional_iterator_tag,TObject*>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<bidirectional_iterator_tag,TObject*,long>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("iterator<bidirectional_iterator_tag,TObject*,long,const TObject**>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<std::string,TObjArray*>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<string,TObjArray*>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<string,TObjArray*>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("map<string,TObjArray*,less<string> >",117,G__get_linked_tagnum(&G__VersionManager_DictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<std::string>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlEstringcOallocatorlEstringgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<string>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlEstringcOallocatorlEstringgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_reverse_iteratorlEvectorlEstringcOallocatorlEstringgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlEstringcOallocatorlEstringgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_reverse_iteratorlEvectorlEstringcOallocatorlEstringgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlEstringcOallocatorlEstringgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<string>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlEstringcOallocatorlEstringgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<ProdId_h>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlEintcOallocatorlEintgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_reverse_iteratorlEvectorlEintcOallocatorlEintgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlEintcOallocatorlEintgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_reverse_iteratorlEvectorlEintcOallocatorlEintgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlEintcOallocatorlEintgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<int>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlEintcOallocatorlEintgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("vector<PackageVersion*>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlEPackageVersionmUcOallocatorlEPackageVersionmUgRsPgR),0,-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<const_iterator>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_reverse_iteratorlEvectorlEPackageVersionmUcOallocatorlEPackageVersionmUgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlEPackageVersionmUcOallocatorlEPackageVersionmUgRsPgR));
   G__setnewtype(-1,NULL,0);
   G__search_typename2("reverse_iterator<iterator>",117,G__get_linked_tagnum(&G__VersionManager_DictLN_reverse_iteratorlEvectorlEPackageVersionmUcOallocatorlEPackageVersionmUgRsPgRcLcLiteratorgR),0,G__get_linked_tagnum(&G__VersionManager_DictLN_vectorlEPackageVersionmUcOallocatorlEPackageVersionmUgRsPgR));
   G__setnewtype(-1,NULL,0);
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */

   /* PackageVersion */
static void G__setup_memvarPackageVersion(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__VersionManager_DictLN_PackageVersion));
   { PackageVersion *p; p=(PackageVersion*)0x1000; if (p) { }
   G__memvar_setup((void*)0,85,0,0,G__get_linked_tagnum(&G__VersionManager_DictLN_TClass),-1,-2,4,"fgIsA=",0,(char*)NULL);
   G__memvar_setup((void*)0,117,0,0,G__get_linked_tagnum(&G__VersionManager_DictLN_string),-1,-1,2,"_name=",0,(char*)NULL);
   G__memvar_setup((void*)0,117,0,0,G__get_linked_tagnum(&G__VersionManager_DictLN_string),-1,-1,2,"_version=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}

extern "C" void G__cpp_setup_memvarVersionManager_Dict() {
}
/***********************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
***********************************************************/

/*********************************************************
* Member function information setup for each class
*********************************************************/
static void G__setup_memfuncPackageVersion(void) {
   /* PackageVersion */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__VersionManager_DictLN_PackageVersion));
   G__memfunc_setup("PackageVersion",1426,G__VersionManager_Dict_496_0_1, 105, G__get_linked_tagnum(&G__VersionManager_DictLN_PackageVersion), -1, 0, 0, 1, 1, 0, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("PackageVersion",1426,G__VersionManager_Dict_496_0_2, 105, G__get_linked_tagnum(&G__VersionManager_DictLN_PackageVersion), -1, 0, 2, 1, 1, 0, 
"u 'string' - 11 - name u 'string' - 11 - version", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("GetName",673,(G__InterfaceMethod) NULL,67, -1, -1, 0, 0, 1, 1, 9, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Name",385,G__VersionManager_Dict_496_0_4, 117, G__get_linked_tagnum(&G__VersionManager_DictLN_string), -1, 1, 0, 1, 1, 9, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("SetName",685,G__VersionManager_Dict_496_0_5, 121, -1, -1, 0, 1, 1, 1, 0, "u 'string' - 11 - name", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Version",742,G__VersionManager_Dict_496_0_6, 117, G__get_linked_tagnum(&G__VersionManager_DictLN_string), -1, 1, 0, 1, 1, 9, "", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("SetVersion",1042,G__VersionManager_Dict_496_0_7, 121, -1, -1, 0, 1, 1, 1, 0, "u 'string' - 11 - version", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("Class",502,G__VersionManager_Dict_496_0_8, 85, G__get_linked_tagnum(&G__VersionManager_DictLN_TClass), -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (TClass* (*)())(&PackageVersion::Class) ), 0);
   G__memfunc_setup("Class_Name",982,G__VersionManager_Dict_496_0_9, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&PackageVersion::Class_Name) ), 0);
   G__memfunc_setup("Class_Version",1339,G__VersionManager_Dict_496_0_10, 115, -1, G__defined_typename("Version_t"), 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (Version_t (*)())(&PackageVersion::Class_Version) ), 0);
   G__memfunc_setup("Dictionary",1046,G__VersionManager_Dict_496_0_11, 121, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (void (*)())(&PackageVersion::Dictionary) ), 0);
   G__memfunc_setup("IsA",253,(G__InterfaceMethod) NULL,85, G__get_linked_tagnum(&G__VersionManager_DictLN_TClass), -1, 0, 0, 1, 1, 8, "", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("ShowMembers",1132,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TMemberInspector' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("Streamer",835,(G__InterfaceMethod) NULL,121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - -", (char*)NULL, (void*) NULL, 1);
   G__memfunc_setup("StreamerNVirtual",1656,G__VersionManager_Dict_496_0_15, 121, -1, -1, 0, 1, 1, 1, 0, "u 'TBuffer' - 1 - ClassDef_StreamerNVirtual_b", (char*)NULL, (void*) NULL, 0);
   G__memfunc_setup("DeclFileName",1145,G__VersionManager_Dict_496_0_16, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&PackageVersion::DeclFileName) ), 0);
   G__memfunc_setup("ImplFileLine",1178,G__VersionManager_Dict_496_0_17, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&PackageVersion::ImplFileLine) ), 0);
   G__memfunc_setup("ImplFileName",1171,G__VersionManager_Dict_496_0_18, 67, -1, -1, 0, 0, 3, 1, 1, "", (char*)NULL, (void*) G__func2void( (const char* (*)())(&PackageVersion::ImplFileName) ), 0);
   G__memfunc_setup("DeclFileLine",1152,G__VersionManager_Dict_496_0_19, 105, -1, -1, 0, 0, 3, 1, 0, "", (char*)NULL, (void*) G__func2void( (int (*)())(&PackageVersion::DeclFileLine) ), 0);
   // automatic copy constructor
   G__memfunc_setup("PackageVersion", 1426, G__VersionManager_Dict_496_0_20, (int) ('i'), G__get_linked_tagnum(&G__VersionManager_DictLN_PackageVersion), -1, 0, 1, 1, 1, 0, "u 'PackageVersion' - 11 - -", (char*) NULL, (void*) NULL, 0);
   // automatic destructor
   G__memfunc_setup("~PackageVersion", 1552, G__VersionManager_Dict_496_0_21, (int) ('y'), -1, -1, 0, 0, 1, 1, 0, "", (char*) NULL, (void*) NULL, 1);
   // automatic assignment operator
   G__memfunc_setup("operator=", 937, G__VersionManager_Dict_496_0_22, (int) ('u'), G__get_linked_tagnum(&G__VersionManager_DictLN_PackageVersion), -1, 1, 1, 1, 1, 0, "u 'PackageVersion' - 11 - -", (char*) NULL, (void*) NULL, 0);
   G__tag_memfunc_reset();
}


/*********************************************************
* Member function information setup
*********************************************************/
extern "C" void G__cpp_setup_memfuncVersionManager_Dict() {
}

/*********************************************************
* Global variable information setup for each class
*********************************************************/
static void G__cpp_setup_global0() {

   /* Setting up global variables */
   G__resetplocal();

}

static void G__cpp_setup_global1() {

   G__resetglobalenv();
}
extern "C" void G__cpp_setup_globalVersionManager_Dict() {
  G__cpp_setup_global0();
  G__cpp_setup_global1();
}

/*********************************************************
* Global function information setup for each class
*********************************************************/
static void G__cpp_setup_func0() {
   G__lastifuncposition();

}

static void G__cpp_setup_func1() {
}

static void G__cpp_setup_func2() {
}

static void G__cpp_setup_func3() {
}

static void G__cpp_setup_func4() {
}

static void G__cpp_setup_func5() {
}

static void G__cpp_setup_func6() {
}

static void G__cpp_setup_func7() {
}

static void G__cpp_setup_func8() {
}

static void G__cpp_setup_func9() {
}

static void G__cpp_setup_func10() {
}

static void G__cpp_setup_func11() {
}

static void G__cpp_setup_func12() {
}

static void G__cpp_setup_func13() {
}

static void G__cpp_setup_func14() {
}

static void G__cpp_setup_func15() {
}

static void G__cpp_setup_func16() {
}

static void G__cpp_setup_func17() {
}

static void G__cpp_setup_func18() {
}

static void G__cpp_setup_func19() {

   G__resetifuncposition();
}

extern "C" void G__cpp_setup_funcVersionManager_Dict() {
  G__cpp_setup_func0();
  G__cpp_setup_func1();
  G__cpp_setup_func2();
  G__cpp_setup_func3();
  G__cpp_setup_func4();
  G__cpp_setup_func5();
  G__cpp_setup_func6();
  G__cpp_setup_func7();
  G__cpp_setup_func8();
  G__cpp_setup_func9();
  G__cpp_setup_func10();
  G__cpp_setup_func11();
  G__cpp_setup_func12();
  G__cpp_setup_func13();
  G__cpp_setup_func14();
  G__cpp_setup_func15();
  G__cpp_setup_func16();
  G__cpp_setup_func17();
  G__cpp_setup_func18();
  G__cpp_setup_func19();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__VersionManager_DictLN_TClass = { "TClass" , 99 , -1 };
G__linked_taginfo G__VersionManager_DictLN_TBuffer = { "TBuffer" , 99 , -1 };
G__linked_taginfo G__VersionManager_DictLN_TMemberInspector = { "TMemberInspector" , 99 , -1 };
G__linked_taginfo G__VersionManager_DictLN_TObject = { "TObject" , 99 , -1 };
G__linked_taginfo G__VersionManager_DictLN_string = { "string" , 99 , -1 };
G__linked_taginfo G__VersionManager_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR = { "vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >" , 99 , -1 };
G__linked_taginfo G__VersionManager_DictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR = { "reverse_iterator<vector<ROOT::TSchemaHelper,allocator<ROOT::TSchemaHelper> >::iterator>" , 99 , -1 };
G__linked_taginfo G__VersionManager_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR = { "vector<TVirtualArray*,allocator<TVirtualArray*> >" , 99 , -1 };
G__linked_taginfo G__VersionManager_DictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR = { "reverse_iterator<vector<TVirtualArray*,allocator<TVirtualArray*> >::iterator>" , 99 , -1 };
G__linked_taginfo G__VersionManager_DictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR = { "iterator<bidirectional_iterator_tag,TObject*,long,const TObject**,const TObject*&>" , 115 , -1 };
G__linked_taginfo G__VersionManager_DictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR = { "map<string,TObjArray*,less<string>,allocator<pair<const string,TObjArray*> > >" , 99 , -1 };
G__linked_taginfo G__VersionManager_DictLN_PackageVersion = { "PackageVersion" , 99 , -1 };
G__linked_taginfo G__VersionManager_DictLN_vectorlEstringcOallocatorlEstringgRsPgR = { "vector<string,allocator<string> >" , 99 , -1 };
G__linked_taginfo G__VersionManager_DictLN_reverse_iteratorlEvectorlEstringcOallocatorlEstringgRsPgRcLcLiteratorgR = { "reverse_iterator<vector<string,allocator<string> >::iterator>" , 99 , -1 };
G__linked_taginfo G__VersionManager_DictLN_vectorlEintcOallocatorlEintgRsPgR = { "vector<int,allocator<int> >" , 99 , -1 };
G__linked_taginfo G__VersionManager_DictLN_reverse_iteratorlEvectorlEintcOallocatorlEintgRsPgRcLcLiteratorgR = { "reverse_iterator<vector<int,allocator<int> >::iterator>" , 99 , -1 };
G__linked_taginfo G__VersionManager_DictLN_vectorlEPackageVersionmUcOallocatorlEPackageVersionmUgRsPgR = { "vector<PackageVersion*,allocator<PackageVersion*> >" , 99 , -1 };
G__linked_taginfo G__VersionManager_DictLN_reverse_iteratorlEvectorlEPackageVersionmUcOallocatorlEPackageVersionmUgRsPgRcLcLiteratorgR = { "reverse_iterator<vector<PackageVersion*,allocator<PackageVersion*> >::iterator>" , 99 , -1 };

/* Reset class/struct taginfo */
extern "C" void G__cpp_reset_tagtableVersionManager_Dict() {
  G__VersionManager_DictLN_TClass.tagnum = -1 ;
  G__VersionManager_DictLN_TBuffer.tagnum = -1 ;
  G__VersionManager_DictLN_TMemberInspector.tagnum = -1 ;
  G__VersionManager_DictLN_TObject.tagnum = -1 ;
  G__VersionManager_DictLN_string.tagnum = -1 ;
  G__VersionManager_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR.tagnum = -1 ;
  G__VersionManager_DictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__VersionManager_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR.tagnum = -1 ;
  G__VersionManager_DictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__VersionManager_DictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR.tagnum = -1 ;
  G__VersionManager_DictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR.tagnum = -1 ;
  G__VersionManager_DictLN_PackageVersion.tagnum = -1 ;
  G__VersionManager_DictLN_vectorlEstringcOallocatorlEstringgRsPgR.tagnum = -1 ;
  G__VersionManager_DictLN_reverse_iteratorlEvectorlEstringcOallocatorlEstringgRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__VersionManager_DictLN_vectorlEintcOallocatorlEintgRsPgR.tagnum = -1 ;
  G__VersionManager_DictLN_reverse_iteratorlEvectorlEintcOallocatorlEintgRsPgRcLcLiteratorgR.tagnum = -1 ;
  G__VersionManager_DictLN_vectorlEPackageVersionmUcOallocatorlEPackageVersionmUgRsPgR.tagnum = -1 ;
  G__VersionManager_DictLN_reverse_iteratorlEvectorlEPackageVersionmUcOallocatorlEPackageVersionmUgRsPgRcLcLiteratorgR.tagnum = -1 ;
}


extern "C" void G__cpp_setup_tagtableVersionManager_Dict() {

   /* Setting up class,struct,union tag entry */
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_TClass);
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_TBuffer);
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_TMemberInspector);
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_TObject);
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_string);
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_vectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgR);
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_reverse_iteratorlEvectorlEROOTcLcLTSchemaHelpercOallocatorlEROOTcLcLTSchemaHelpergRsPgRcLcLiteratorgR);
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_vectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgR);
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_reverse_iteratorlEvectorlETVirtualArraymUcOallocatorlETVirtualArraymUgRsPgRcLcLiteratorgR);
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_iteratorlEbidirectional_iterator_tagcOTObjectmUcOlongcOconstsPTObjectmUmUcOconstsPTObjectmUaNgR);
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_maplEstringcOTObjArraymUcOlesslEstringgRcOallocatorlEpairlEconstsPstringcOTObjArraymUgRsPgRsPgR);
   G__tagtable_setup(G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_PackageVersion),sizeof(PackageVersion),-1,324864,(char*)NULL,G__setup_memvarPackageVersion,G__setup_memfuncPackageVersion);
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_vectorlEstringcOallocatorlEstringgRsPgR);
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_reverse_iteratorlEvectorlEstringcOallocatorlEstringgRsPgRcLcLiteratorgR);
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_vectorlEintcOallocatorlEintgRsPgR);
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_reverse_iteratorlEvectorlEintcOallocatorlEintgRsPgRcLcLiteratorgR);
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_vectorlEPackageVersionmUcOallocatorlEPackageVersionmUgRsPgR);
   G__get_linked_tagnum_fwd(&G__VersionManager_DictLN_reverse_iteratorlEvectorlEPackageVersionmUcOallocatorlEPackageVersionmUgRsPgRcLcLiteratorgR);
}
extern "C" void G__cpp_setupVersionManager_Dict(void) {
  G__check_setup_version(30051515,"G__cpp_setupVersionManager_Dict()");
  G__set_cpp_environmentVersionManager_Dict();
  G__cpp_setup_tagtableVersionManager_Dict();

  G__cpp_setup_inheritanceVersionManager_Dict();

  G__cpp_setup_typetableVersionManager_Dict();

  G__cpp_setup_memvarVersionManager_Dict();

  G__cpp_setup_memfuncVersionManager_Dict();
  G__cpp_setup_globalVersionManager_Dict();
  G__cpp_setup_funcVersionManager_Dict();

   if(0==G__getsizep2memfunc()) G__get_sizep2memfuncVersionManager_Dict();
  return;
}
class G__cpp_setup_initVersionManager_Dict {
  public:
    G__cpp_setup_initVersionManager_Dict() { G__add_setup_func("VersionManager_Dict",(G__incsetup)(&G__cpp_setupVersionManager_Dict)); G__call_setup_funcs(); }
   ~G__cpp_setup_initVersionManager_Dict() { G__remove_setup_func("VersionManager_Dict"); }
};
G__cpp_setup_initVersionManager_Dict G__cpp_setup_initializerVersionManager_Dict;

