; ModuleID = 'external.cpp'
target datalayout = "e-m:x-p:32:32-i64:64-f80:32-n8:16:32-S32"
target triple = "i686-pc-windows-msvc"

%"class.std::_Generic_error_category" = type { %"class.std::error_category" }
%"class.std::error_category" = type { i32 (...)** }
%"class.std::_Iostream_error_category" = type { %"class.std::_Generic_error_category" }
%"class.std::_System_error_category" = type { %"class.std::_Generic_error_category" }
%rtti.CompleteObjectLocator = type { i32, i32, i32, i8*, %rtti.ClassHierarchyDescriptor* }
%rtti.ClassHierarchyDescriptor = type { i32, i32, i32, %rtti.BaseClassDescriptor** }
%rtti.BaseClassDescriptor = type { i8*, i32, i32, i32, i32, i32, %rtti.ClassHierarchyDescriptor* }
%rtti.TypeDescriptor33 = type { i8**, i8*, [34 x i8] }
%rtti.TypeDescriptor24 = type { i8**, i8*, [25 x i8] }
%rtti.TypeDescriptor34 = type { i8**, i8*, [35 x i8] }
%rtti.TypeDescriptor32 = type { i8**, i8*, [33 x i8] }
%struct._iobuf = type { i8*, i32, i8*, i32, i32, i32, i32, i8* }
%struct.SPE_SYSTEM_INFO = type { i32, i32, i8*, i8*, i32, i32, i32, i32, i16, i16 }
%struct._SYSTEM_INFO = type { %union.anon, i32, i8*, i8*, i32, i32, i32, i32, i16, i16 }
%union.anon = type { i32 }
%struct._OSVERSIONINFOA = type { i32, i32, i32, i32, i32, [128 x i8] }
%struct._RTL_CRITICAL_SECTION = type { %struct._RTL_CRITICAL_SECTION_DEBUG*, i32, i32, i8*, i8*, i32 }
%struct._RTL_CRITICAL_SECTION_DEBUG = type { i16, i16, %struct._RTL_CRITICAL_SECTION*, %struct._LIST_ENTRY, i32, i32, i32, i16, i16 }
%struct._LIST_ENTRY = type { %struct._LIST_ENTRY*, %struct._LIST_ENTRY* }
%struct.timecaps_tag = type { i32, i32 }
%struct.tm = type { i32, i32, i32, i32, i32, i32, i32, i32, i32 }
%struct._SECURITY_ATTRIBUTES = type { i32, i8*, i32 }
%struct.HINSTANCE__ = type { i32 }
%struct._SYSTEMTIME = type { i16, i16, i16, i16, i16, i16, i16, i16 }
%struct._FILETIME = type { i32, i32 }
%struct._OSVERSIONINFOW = type { i32, i32, i32, i32, i32, [128 x i16] }
%struct._OVERLAPPED = type { i32, i32, %union.anon.0, i8* }
%union.anon.0 = type { %struct.anon.1 }
%struct.anon.1 = type { i32, i32 }
%union._LARGE_INTEGER = type { i64 }
%"class.std::basic_string" = type { %"class.std::_String_alloc" }
%"class.std::_String_alloc" = type { %"class.std::_String_val" }
%"class.std::_String_val" = type { %"union.std::_String_val<std::_Simple_types<char> >::_Bxty", i32, i32 }
%"union.std::_String_val<std::_Simple_types<char> >::_Bxty" = type { i8*, [12 x i8] }
%"class.std::error_condition" = type { i32, %"class.std::error_category"* }
%"class.std::error_code" = type { i32, %"class.std::error_category"* }
%"class.std::allocator" = type { i8 }
%"struct.std::_Allocator_base" = type { i8 }
%"struct.std::_Wrap_alloc" = type { i8 }
%"struct.std::_Container_base0" = type { i8 }
%eh.ThrowInfo = type { i32, i8*, i8*, i8* }

$"\01??__E_Generic_object@?$_Error_objects@H@std@@2V_Generic_error_category@2@A@YAXXZ" = comdat any

$"\01??0_Generic_error_category@std@@QAE@XZ" = comdat any

$"\01??1_Generic_error_category@std@@UAE@XZ" = comdat any

$"\01??__E_Iostream_object@?$_Error_objects@H@std@@2V_Iostream_error_category@2@A@YAXXZ" = comdat any

$"\01??0_Iostream_error_category@std@@QAE@XZ" = comdat any

$"\01??1_Iostream_error_category@std@@UAE@XZ" = comdat any

$"\01??__E_System_object@?$_Error_objects@H@std@@2V_System_error_category@2@A@YAXXZ" = comdat any

$"\01??0_System_error_category@std@@QAE@XZ" = comdat any

$"\01??1_System_error_category@std@@UAE@XZ" = comdat any

$"\01??0error_category@std@@QAE@XZ" = comdat any

$"\01??_G_Generic_error_category@std@@UAEPAXI@Z" = comdat any

$"\01?name@_Generic_error_category@std@@UBEPBDXZ" = comdat any

$"\01?message@_Generic_error_category@std@@UBE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@2@H@Z" = comdat any

$"\01?default_error_condition@error_category@std@@UBE?AVerror_condition@2@H@Z" = comdat any

$"\01?equivalent@error_category@std@@UBE_NABVerror_code@2@H@Z" = comdat any

$"\01?equivalent@error_category@std@@UBE_NHABVerror_condition@2@@Z" = comdat any

$"\01??_Gerror_category@std@@UAEPAXI@Z" = comdat any

$"\01??1error_category@std@@UAE@XZ" = comdat any

$"\01??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z" = comdat any

$"\01??0?$allocator@D@std@@QAE@XZ" = comdat any

$"\01??0?$_String_alloc@$0A@U?$_String_base_types@DV?$allocator@D@std@@@std@@@std@@QAE@ABV?$allocator@D@1@@Z" = comdat any

$"\01?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEX_NI@Z" = comdat any

$"\01?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBD@Z" = comdat any

$"\01??0?$_String_val@U?$_Simple_types@D@std@@@std@@QAE@XZ" = comdat any

$"\01?_Getal@?$_String_alloc@$0A@U?$_String_base_types@DV?$allocator@D@std@@@std@@@std@@QBE?AU?$_Wrap_alloc@V?$allocator@D@std@@@2@XZ" = comdat any

$"\01??$destroy@PAD@?$_Wrap_alloc@V?$allocator@D@std@@@std@@QAEXPAPAD@Z" = comdat any

$"\01?copy@?$char_traits@D@std@@SAPADPADPBDI@Z" = comdat any

$"\01??$addressof@D@std@@YAPADAAD@Z" = comdat any

$"\01?deallocate@?$_Wrap_alloc@V?$allocator@D@std@@@std@@QAEXPADI@Z" = comdat any

$"\01?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEXI@Z" = comdat any

$"\01??0?$_Wrap_alloc@V?$allocator@D@std@@@std@@QAE@XZ" = comdat any

$"\01??$destroy@PAD@?$allocator_traits@V?$allocator@D@std@@@std@@SAXAAV?$allocator@D@1@PAPAD@Z" = comdat any

$"\01??$destroy@PAD@?$allocator@D@std@@QAEXPAPAD@Z" = comdat any

$"\01?deallocate@?$allocator@D@std@@QAEXPADI@Z" = comdat any

$"\01?assign@?$char_traits@D@std@@SAXAADABD@Z" = comdat any

$"\01?_Myptr@?$_String_val@U?$_Simple_types@D@std@@@std@@QAEPADXZ" = comdat any

$"\01?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBDI@Z" = comdat any

$"\01?length@?$char_traits@D@std@@SAIPBD@Z" = comdat any

$"\01?_Inside@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE_NPBD@Z" = comdat any

$"\01?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@ABV12@II@Z" = comdat any

$"\01?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE_NI_N@Z" = comdat any

$"\01?size@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ" = comdat any

$"\01?_Xran@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEXXZ" = comdat any

$"\01?erase@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@I@Z" = comdat any

$"\01?erase@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@II@Z" = comdat any

$"\01?_Myptr@?$_String_val@U?$_Simple_types@D@std@@@std@@QBEPBDXZ" = comdat any

$"\01?move@?$char_traits@D@std@@SAPADPADPBDI@Z" = comdat any

$"\01?max_size@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ" = comdat any

$"\01?_Xlen@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEXXZ" = comdat any

$"\01?_Copy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEXII@Z" = comdat any

$"\01?max_size@?$_Wrap_alloc@V?$allocator@D@std@@@std@@QBEIXZ" = comdat any

$"\01?max_size@?$allocator_traits@V?$allocator@D@std@@@std@@SAIABV?$allocator@D@2@@Z" = comdat any

$"\01?max_size@?$allocator@D@std@@QBEIXZ" = comdat any

$"\01?allocate@?$_Wrap_alloc@V?$allocator@D@std@@@std@@QAEPADI@Z" = comdat any

$"\01??$construct@PADAAPAD@?$_Wrap_alloc@V?$allocator@D@std@@@std@@QAEXPAPADAAPAD@Z" = comdat any

$"\01?allocate@?$allocator@D@std@@QAEPADI@Z" = comdat any

$"\01??$_Allocate@D@std@@YAPADIPAD@Z" = comdat any

$"\01??$construct@PADAAPAD@?$allocator_traits@V?$allocator@D@std@@@std@@SAXAAV?$allocator@D@1@PAPADAAPAD@Z" = comdat any

$"\01??$forward@AAPAD@std@@YAAAPADAAPAD@Z" = comdat any

$"\01??$construct@PADAAPAD@?$allocator@D@std@@QAEXPAPADAAPAD@Z" = comdat any

$"\01??0error_condition@std@@QAE@HABVerror_category@1@@Z" = comdat any

$"\01??8error_category@std@@QBE_NABV01@@Z" = comdat any

$"\01?category@error_code@std@@QBEABVerror_category@2@XZ" = comdat any

$"\01?value@error_code@std@@QBEHXZ" = comdat any

$"\01??8error_condition@std@@QBE_NABV01@@Z" = comdat any

$"\01?category@error_condition@std@@QBEABVerror_category@2@XZ" = comdat any

$"\01?value@error_condition@std@@QBEHXZ" = comdat any

$"\01??_G_Iostream_error_category@std@@UAEPAXI@Z" = comdat any

$"\01?name@_Iostream_error_category@std@@UBEPBDXZ" = comdat any

$"\01?message@_Iostream_error_category@std@@UBE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@2@H@Z" = comdat any

$"\01??_G_System_error_category@std@@UAEPAXI@Z" = comdat any

$"\01?name@_System_error_category@std@@UBEPBDXZ" = comdat any

$"\01?message@_System_error_category@std@@UBE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@2@H@Z" = comdat any

$"\01?default_error_condition@_System_error_category@std@@UBE?AVerror_condition@2@H@Z" = comdat any

$"\01?generic_category@std@@YAABVerror_category@1@XZ" = comdat any

$"\01?system_category@std@@YAABVerror_category@1@XZ" = comdat any

$"\01??_C@_0BJ@DMDFJPHM@?$CK?$CK?$CK?$CK?$CK?$CK?$CK?$CKlongJmp?$CK?$CK?$CK?$CK?$CK?$CK?$CK?$CK?$CK?$AA@" = comdat any

$"\01??_C@_0BJ@DOBLNIJM@This?5path?5cost?5?3?5?$CFlu?5ms?6?$AA@" = comdat any

$"\01??_C@_03OFAPEBGM@?$CFs?6?$AA@" = comdat any

$"\01??_C@_03BAOKKJLE@afd?$AA@" = comdat any

$"\01??_C@_04JIHMPGLA@asdf?$AA@" = comdat any

$"\01??_C@_01MCMALHOG@a?$AA@" = comdat any

$"\01??_C@_0N@MMJFHEIA@IRResult?4exe?$AA@" = comdat any

$"\01??_C@_03MNDEGCIG@xxx?$AA@" = comdat any

$"\01??_C@_01FJMABOPO@x?$AA@" = comdat any

$"\01??_C@_01KDCPPGHE@r?$AA@" = comdat any

$"\01??_C@_05KAGKJOA@1?4txt?$AA@" = comdat any

$"\01??_C@_02DPKJAMEF@?$CFd?$AA@" = comdat any

$"\01??_C@_02DKCKIIND@?$CFs?$AA@" = comdat any

$"\01??_C@_04HLKDGPAO@sdfs?$AA@" = comdat any

$"\01??_C@_00CNPNBAHC@?$AA@" = comdat any

$"\01??_C@_02NJPGOMH@?$CFf?$AA@" = comdat any

$"\01?_Generic_object@?$_Error_objects@H@std@@2V_Generic_error_category@2@A" = comdat any

$"\01?_Iostream_object@?$_Error_objects@H@std@@2V_Iostream_error_category@2@A" = comdat any

$"\01?_System_object@?$_Error_objects@H@std@@2V_System_error_category@2@A" = comdat any

$"\01??_7_Generic_error_category@std@@6B@" = comdat largest

$"\01??_R4_Generic_error_category@std@@6B@" = comdat any

$"\01??_R0?AV_Generic_error_category@std@@@8" = comdat any

$"\01??_R3_Generic_error_category@std@@8" = comdat any

$"\01??_R2_Generic_error_category@std@@8" = comdat any

$"\01??_R1A@?0A@EA@_Generic_error_category@std@@8" = comdat any

$"\01??_R1A@?0A@EA@error_category@std@@8" = comdat any

$"\01??_R0?AVerror_category@std@@@8" = comdat any

$"\01??_R3error_category@std@@8" = comdat any

$"\01??_R2error_category@std@@8" = comdat any

$"\01??_7error_category@std@@6B@" = comdat largest

$"\01??_R4error_category@std@@6B@" = comdat any

$"\01??_C@_07DCLBNMLN@generic?$AA@" = comdat any

$"\01??_C@_0O@BFJCFAAK@unknown?5error?$AA@" = comdat any

$"\01??_C@_0BI@CFPLBAOH@invalid?5string?5position?$AA@" = comdat any

$"\01??_C@_0BA@JFNIOLAK@string?5too?5long?$AA@" = comdat any

$"\01??_7_Iostream_error_category@std@@6B@" = comdat largest

$"\01??_R4_Iostream_error_category@std@@6B@" = comdat any

$"\01??_R0?AV_Iostream_error_category@std@@@8" = comdat any

$"\01??_R3_Iostream_error_category@std@@8" = comdat any

$"\01??_R2_Iostream_error_category@std@@8" = comdat any

$"\01??_R1A@?0A@EA@_Iostream_error_category@std@@8" = comdat any

$"\01??_C@_08LLGCOLLL@iostream?$AA@" = comdat any

$"\01??_C@_0BG@PADBLCHM@iostream?5stream?5error?$AA@" = comdat any

$"\01??_7_System_error_category@std@@6B@" = comdat largest

$"\01??_R4_System_error_category@std@@6B@" = comdat any

$"\01??_R0?AV_System_error_category@std@@@8" = comdat any

$"\01??_R3_System_error_category@std@@8" = comdat any

$"\01??_R2_System_error_category@std@@8" = comdat any

$"\01??_R1A@?0A@EA@_System_error_category@std@@8" = comdat any

$"\01??_C@_06FHFOAHML@system?$AA@" = comdat any

@"\01?start_time@@3KA" = global i32 0, align 4
@"\01?$$myJmpbuf@@3PAHA" = global [16 x i32] zeroinitializer, align 4
@"\01??_C@_0BJ@DMDFJPHM@?$CK?$CK?$CK?$CK?$CK?$CK?$CK?$CKlongJmp?$CK?$CK?$CK?$CK?$CK?$CK?$CK?$CK?$CK?$AA@" = linkonce_odr unnamed_addr constant [25 x i8] c"********longJmp*********\00", comdat, align 1
@"\01??_C@_0BJ@DOBLNIJM@This?5path?5cost?5?3?5?$CFlu?5ms?6?$AA@" = linkonce_odr unnamed_addr constant [25 x i8] c"This path cost : %lu ms\0A\00", comdat, align 1
@"\01?j_buf@@3PAHA" = global [16 x i32] zeroinitializer, align 4
@"\01??_C@_03OFAPEBGM@?$CFs?6?$AA@" = linkonce_odr unnamed_addr constant [4 x i8] c"%s\0A\00", comdat, align 1
@"\01??_C@_03BAOKKJLE@afd?$AA@" = linkonce_odr unnamed_addr constant [4 x i8] c"afd\00", comdat, align 1
@"\01??_C@_04JIHMPGLA@asdf?$AA@" = linkonce_odr unnamed_addr constant [5 x i8] c"asdf\00", comdat, align 1
@"\01?c@?1??ex@@YAXXZ@3PADA" = private unnamed_addr constant [10 x i8] c"a\00\00\00\00\00\00\00\00\00", align 1
@"\01?b@?1??ex@@YAXXZ@3PADA" = private unnamed_addr constant [10 x i8] c"a\00\00\00\00\00\00\00\00\00", align 1
@"\01??_C@_01MCMALHOG@a?$AA@" = linkonce_odr unnamed_addr constant [2 x i8] c"a\00", comdat, align 1
@"\01??_C@_0N@MMJFHEIA@IRResult?4exe?$AA@" = linkonce_odr unnamed_addr constant [13 x i8] c"IRResult.exe\00", comdat, align 1
@"\01??_C@_03MNDEGCIG@xxx?$AA@" = linkonce_odr unnamed_addr constant [4 x i8] c"xxx\00", comdat, align 1
@"\01??_C@_01FJMABOPO@x?$AA@" = linkonce_odr unnamed_addr constant [2 x i8] c"x\00", comdat, align 1
@"\01??_C@_01KDCPPGHE@r?$AA@" = linkonce_odr unnamed_addr constant [2 x i8] c"r\00", comdat, align 1
@"\01??_C@_05KAGKJOA@1?4txt?$AA@" = linkonce_odr unnamed_addr constant [6 x i8] c"1.txt\00", comdat, align 1
@"\01??_C@_02DPKJAMEF@?$CFd?$AA@" = linkonce_odr unnamed_addr constant [3 x i8] c"%d\00", comdat, align 1
@"\01??_C@_02DKCKIIND@?$CFs?$AA@" = linkonce_odr unnamed_addr constant [3 x i8] c"%s\00", comdat, align 1
@"\01??_C@_04HLKDGPAO@sdfs?$AA@" = linkonce_odr unnamed_addr constant [5 x i8] c"sdfs\00", comdat, align 1
@"\01??_C@_00CNPNBAHC@?$AA@" = linkonce_odr unnamed_addr constant [1 x i8] zeroinitializer, comdat, align 1
@"\01?$$tempLen@@3HA" = global i32 0, align 4
@"\01?$$varsInfo@@3PADA" = global [100000 x i8] zeroinitializer, align 1
@"\01??_C@_02NJPGOMH@?$CFf?$AA@" = linkonce_odr unnamed_addr constant [3 x i8] c"%f\00", comdat, align 1
@"\01?_Generic_object@?$_Error_objects@H@std@@2V_Generic_error_category@2@A" = linkonce_odr global %"class.std::_Generic_error_category" zeroinitializer, comdat, align 4
@"\01?_Iostream_object@?$_Error_objects@H@std@@2V_Iostream_error_category@2@A" = linkonce_odr global %"class.std::_Iostream_error_category" zeroinitializer, comdat, align 4
@"\01?_System_object@?$_Error_objects@H@std@@2V_System_error_category@2@A" = linkonce_odr global %"class.std::_System_error_category" zeroinitializer, comdat, align 4
@0 = private unnamed_addr constant [7 x i8*] [i8* bitcast (%rtti.CompleteObjectLocator* @"\01??_R4_Generic_error_category@std@@6B@" to i8*), i8* bitcast (i8* (%"class.std::_Generic_error_category"*, i32)* @"\01??_G_Generic_error_category@std@@UAEPAXI@Z" to i8*), i8* bitcast (i8* (%"class.std::_Generic_error_category"*)* @"\01?name@_Generic_error_category@std@@UBEPBDXZ" to i8*), i8* bitcast (void (%"class.std::_Generic_error_category"*, %"class.std::basic_string"*, i32)* @"\01?message@_Generic_error_category@std@@UBE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@2@H@Z" to i8*), i8* bitcast (void (%"class.std::error_category"*, %"class.std::error_condition"*, i32)* @"\01?default_error_condition@error_category@std@@UBE?AVerror_condition@2@H@Z" to i8*), i8* bitcast (i1 (%"class.std::error_category"*, %"class.std::error_code"*, i32)* @"\01?equivalent@error_category@std@@UBE_NABVerror_code@2@H@Z" to i8*), i8* bitcast (i1 (%"class.std::error_category"*, i32, %"class.std::error_condition"*)* @"\01?equivalent@error_category@std@@UBE_NHABVerror_condition@2@@Z" to i8*)], comdat($"\01??_7_Generic_error_category@std@@6B@")
@"\01??_R4_Generic_error_category@std@@6B@" = linkonce_odr constant %rtti.CompleteObjectLocator { i32 0, i32 0, i32 0, i8* bitcast (%rtti.TypeDescriptor33* @"\01??_R0?AV_Generic_error_category@std@@@8" to i8*), %rtti.ClassHierarchyDescriptor* @"\01??_R3_Generic_error_category@std@@8" }, comdat
@"\01??_7type_info@@6B@" = external constant i8*
@"\01??_R0?AV_Generic_error_category@std@@@8" = linkonce_odr global %rtti.TypeDescriptor33 { i8** @"\01??_7type_info@@6B@", i8* null, [34 x i8] c".?AV_Generic_error_category@std@@\00" }, comdat
@"\01??_R3_Generic_error_category@std@@8" = linkonce_odr constant %rtti.ClassHierarchyDescriptor { i32 0, i32 0, i32 2, %rtti.BaseClassDescriptor** getelementptr inbounds ([3 x %rtti.BaseClassDescriptor*], [3 x %rtti.BaseClassDescriptor*]* @"\01??_R2_Generic_error_category@std@@8", i32 0, i32 0) }, comdat
@"\01??_R2_Generic_error_category@std@@8" = linkonce_odr constant [3 x %rtti.BaseClassDescriptor*] [%rtti.BaseClassDescriptor* @"\01??_R1A@?0A@EA@_Generic_error_category@std@@8", %rtti.BaseClassDescriptor* @"\01??_R1A@?0A@EA@error_category@std@@8", %rtti.BaseClassDescriptor* null], comdat
@"\01??_R1A@?0A@EA@_Generic_error_category@std@@8" = linkonce_odr constant %rtti.BaseClassDescriptor { i8* bitcast (%rtti.TypeDescriptor33* @"\01??_R0?AV_Generic_error_category@std@@@8" to i8*), i32 1, i32 0, i32 -1, i32 0, i32 64, %rtti.ClassHierarchyDescriptor* @"\01??_R3_Generic_error_category@std@@8" }, comdat
@"\01??_R1A@?0A@EA@error_category@std@@8" = linkonce_odr constant %rtti.BaseClassDescriptor { i8* bitcast (%rtti.TypeDescriptor24* @"\01??_R0?AVerror_category@std@@@8" to i8*), i32 0, i32 0, i32 -1, i32 0, i32 64, %rtti.ClassHierarchyDescriptor* @"\01??_R3error_category@std@@8" }, comdat
@"\01??_R0?AVerror_category@std@@@8" = linkonce_odr global %rtti.TypeDescriptor24 { i8** @"\01??_7type_info@@6B@", i8* null, [25 x i8] c".?AVerror_category@std@@\00" }, comdat
@"\01??_R3error_category@std@@8" = linkonce_odr constant %rtti.ClassHierarchyDescriptor { i32 0, i32 0, i32 1, %rtti.BaseClassDescriptor** getelementptr inbounds ([2 x %rtti.BaseClassDescriptor*], [2 x %rtti.BaseClassDescriptor*]* @"\01??_R2error_category@std@@8", i32 0, i32 0) }, comdat
@"\01??_R2error_category@std@@8" = linkonce_odr constant [2 x %rtti.BaseClassDescriptor*] [%rtti.BaseClassDescriptor* @"\01??_R1A@?0A@EA@error_category@std@@8", %rtti.BaseClassDescriptor* null], comdat
@1 = private unnamed_addr constant [7 x i8*] [i8* bitcast (%rtti.CompleteObjectLocator* @"\01??_R4error_category@std@@6B@" to i8*), i8* bitcast (i8* (%"class.std::error_category"*, i32)* @"\01??_Gerror_category@std@@UAEPAXI@Z" to i8*), i8* bitcast (void ()* @_purecall to i8*), i8* bitcast (void ()* @_purecall to i8*), i8* bitcast (void (%"class.std::error_category"*, %"class.std::error_condition"*, i32)* @"\01?default_error_condition@error_category@std@@UBE?AVerror_condition@2@H@Z" to i8*), i8* bitcast (i1 (%"class.std::error_category"*, %"class.std::error_code"*, i32)* @"\01?equivalent@error_category@std@@UBE_NABVerror_code@2@H@Z" to i8*), i8* bitcast (i1 (%"class.std::error_category"*, i32, %"class.std::error_condition"*)* @"\01?equivalent@error_category@std@@UBE_NHABVerror_condition@2@@Z" to i8*)], comdat($"\01??_7error_category@std@@6B@")
@"\01??_R4error_category@std@@6B@" = linkonce_odr constant %rtti.CompleteObjectLocator { i32 0, i32 0, i32 0, i8* bitcast (%rtti.TypeDescriptor24* @"\01??_R0?AVerror_category@std@@@8" to i8*), %rtti.ClassHierarchyDescriptor* @"\01??_R3error_category@std@@8" }, comdat
@"\01??_C@_07DCLBNMLN@generic?$AA@" = linkonce_odr unnamed_addr constant [8 x i8] c"generic\00", comdat, align 1
@"\01??_C@_0O@BFJCFAAK@unknown?5error?$AA@" = linkonce_odr unnamed_addr constant [14 x i8] c"unknown error\00", comdat, align 1
@"\01??_C@_0BI@CFPLBAOH@invalid?5string?5position?$AA@" = linkonce_odr unnamed_addr constant [24 x i8] c"invalid string position\00", comdat, align 1
@"\01??_C@_0BA@JFNIOLAK@string?5too?5long?$AA@" = linkonce_odr unnamed_addr constant [16 x i8] c"string too long\00", comdat, align 1
@2 = private unnamed_addr constant [7 x i8*] [i8* bitcast (%rtti.CompleteObjectLocator* @"\01??_R4_Iostream_error_category@std@@6B@" to i8*), i8* bitcast (i8* (%"class.std::_Iostream_error_category"*, i32)* @"\01??_G_Iostream_error_category@std@@UAEPAXI@Z" to i8*), i8* bitcast (i8* (%"class.std::_Iostream_error_category"*)* @"\01?name@_Iostream_error_category@std@@UBEPBDXZ" to i8*), i8* bitcast (void (%"class.std::_Iostream_error_category"*, %"class.std::basic_string"*, i32)* @"\01?message@_Iostream_error_category@std@@UBE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@2@H@Z" to i8*), i8* bitcast (void (%"class.std::error_category"*, %"class.std::error_condition"*, i32)* @"\01?default_error_condition@error_category@std@@UBE?AVerror_condition@2@H@Z" to i8*), i8* bitcast (i1 (%"class.std::error_category"*, %"class.std::error_code"*, i32)* @"\01?equivalent@error_category@std@@UBE_NABVerror_code@2@H@Z" to i8*), i8* bitcast (i1 (%"class.std::error_category"*, i32, %"class.std::error_condition"*)* @"\01?equivalent@error_category@std@@UBE_NHABVerror_condition@2@@Z" to i8*)], comdat($"\01??_7_Iostream_error_category@std@@6B@")
@"\01??_R4_Iostream_error_category@std@@6B@" = linkonce_odr constant %rtti.CompleteObjectLocator { i32 0, i32 0, i32 0, i8* bitcast (%rtti.TypeDescriptor34* @"\01??_R0?AV_Iostream_error_category@std@@@8" to i8*), %rtti.ClassHierarchyDescriptor* @"\01??_R3_Iostream_error_category@std@@8" }, comdat
@"\01??_R0?AV_Iostream_error_category@std@@@8" = linkonce_odr global %rtti.TypeDescriptor34 { i8** @"\01??_7type_info@@6B@", i8* null, [35 x i8] c".?AV_Iostream_error_category@std@@\00" }, comdat
@"\01??_R3_Iostream_error_category@std@@8" = linkonce_odr constant %rtti.ClassHierarchyDescriptor { i32 0, i32 0, i32 3, %rtti.BaseClassDescriptor** getelementptr inbounds ([4 x %rtti.BaseClassDescriptor*], [4 x %rtti.BaseClassDescriptor*]* @"\01??_R2_Iostream_error_category@std@@8", i32 0, i32 0) }, comdat
@"\01??_R2_Iostream_error_category@std@@8" = linkonce_odr constant [4 x %rtti.BaseClassDescriptor*] [%rtti.BaseClassDescriptor* @"\01??_R1A@?0A@EA@_Iostream_error_category@std@@8", %rtti.BaseClassDescriptor* @"\01??_R1A@?0A@EA@_Generic_error_category@std@@8", %rtti.BaseClassDescriptor* @"\01??_R1A@?0A@EA@error_category@std@@8", %rtti.BaseClassDescriptor* null], comdat
@"\01??_R1A@?0A@EA@_Iostream_error_category@std@@8" = linkonce_odr constant %rtti.BaseClassDescriptor { i8* bitcast (%rtti.TypeDescriptor34* @"\01??_R0?AV_Iostream_error_category@std@@@8" to i8*), i32 2, i32 0, i32 -1, i32 0, i32 64, %rtti.ClassHierarchyDescriptor* @"\01??_R3_Iostream_error_category@std@@8" }, comdat
@"\01??_C@_08LLGCOLLL@iostream?$AA@" = linkonce_odr unnamed_addr constant [9 x i8] c"iostream\00", comdat, align 1
@"\01??_C@_0BG@PADBLCHM@iostream?5stream?5error?$AA@" = linkonce_odr unnamed_addr constant [22 x i8] c"iostream stream error\00", comdat, align 1
@3 = private unnamed_addr constant [7 x i8*] [i8* bitcast (%rtti.CompleteObjectLocator* @"\01??_R4_System_error_category@std@@6B@" to i8*), i8* bitcast (i8* (%"class.std::_System_error_category"*, i32)* @"\01??_G_System_error_category@std@@UAEPAXI@Z" to i8*), i8* bitcast (i8* (%"class.std::_System_error_category"*)* @"\01?name@_System_error_category@std@@UBEPBDXZ" to i8*), i8* bitcast (void (%"class.std::_System_error_category"*, %"class.std::basic_string"*, i32)* @"\01?message@_System_error_category@std@@UBE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@2@H@Z" to i8*), i8* bitcast (void (%"class.std::_System_error_category"*, %"class.std::error_condition"*, i32)* @"\01?default_error_condition@_System_error_category@std@@UBE?AVerror_condition@2@H@Z" to i8*), i8* bitcast (i1 (%"class.std::error_category"*, %"class.std::error_code"*, i32)* @"\01?equivalent@error_category@std@@UBE_NABVerror_code@2@H@Z" to i8*), i8* bitcast (i1 (%"class.std::error_category"*, i32, %"class.std::error_condition"*)* @"\01?equivalent@error_category@std@@UBE_NHABVerror_condition@2@@Z" to i8*)], comdat($"\01??_7_System_error_category@std@@6B@")
@"\01??_R4_System_error_category@std@@6B@" = linkonce_odr constant %rtti.CompleteObjectLocator { i32 0, i32 0, i32 0, i8* bitcast (%rtti.TypeDescriptor32* @"\01??_R0?AV_System_error_category@std@@@8" to i8*), %rtti.ClassHierarchyDescriptor* @"\01??_R3_System_error_category@std@@8" }, comdat
@"\01??_R0?AV_System_error_category@std@@@8" = linkonce_odr global %rtti.TypeDescriptor32 { i8** @"\01??_7type_info@@6B@", i8* null, [33 x i8] c".?AV_System_error_category@std@@\00" }, comdat
@"\01??_R3_System_error_category@std@@8" = linkonce_odr constant %rtti.ClassHierarchyDescriptor { i32 0, i32 0, i32 3, %rtti.BaseClassDescriptor** getelementptr inbounds ([4 x %rtti.BaseClassDescriptor*], [4 x %rtti.BaseClassDescriptor*]* @"\01??_R2_System_error_category@std@@8", i32 0, i32 0) }, comdat
@"\01??_R2_System_error_category@std@@8" = linkonce_odr constant [4 x %rtti.BaseClassDescriptor*] [%rtti.BaseClassDescriptor* @"\01??_R1A@?0A@EA@_System_error_category@std@@8", %rtti.BaseClassDescriptor* @"\01??_R1A@?0A@EA@_Generic_error_category@std@@8", %rtti.BaseClassDescriptor* @"\01??_R1A@?0A@EA@error_category@std@@8", %rtti.BaseClassDescriptor* null], comdat
@"\01??_R1A@?0A@EA@_System_error_category@std@@8" = linkonce_odr constant %rtti.BaseClassDescriptor { i8* bitcast (%rtti.TypeDescriptor32* @"\01??_R0?AV_System_error_category@std@@@8" to i8*), i32 2, i32 0, i32 -1, i32 0, i32 64, %rtti.ClassHierarchyDescriptor* @"\01??_R3_System_error_category@std@@8" }, comdat
@"\01??_C@_06FHFOAHML@system?$AA@" = linkonce_odr unnamed_addr constant [7 x i8] c"system\00", comdat, align 1
@llvm.global_ctors = appending global [3 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @"\01??__E_Generic_object@?$_Error_objects@H@std@@2V_Generic_error_category@2@A@YAXXZ", i8* bitcast (%"class.std::_Generic_error_category"* @"\01?_Generic_object@?$_Error_objects@H@std@@2V_Generic_error_category@2@A" to i8*) }, { i32, void ()*, i8* } { i32 65535, void ()* @"\01??__E_Iostream_object@?$_Error_objects@H@std@@2V_Iostream_error_category@2@A@YAXXZ", i8* bitcast (%"class.std::_Iostream_error_category"* @"\01?_Iostream_object@?$_Error_objects@H@std@@2V_Iostream_error_category@2@A" to i8*) }, { i32, void ()*, i8* } { i32 65535, void ()* @"\01??__E_System_object@?$_Error_objects@H@std@@2V_System_error_category@2@A@YAXXZ", i8* bitcast (%"class.std::_System_error_category"* @"\01?_System_object@?$_Error_objects@H@std@@2V_System_error_category@2@A" to i8*) }]

@"\01??_7_Generic_error_category@std@@6B@" = unnamed_addr alias getelementptr inbounds ([7 x i8*], [7 x i8*]* @0, i32 0, i32 1)
@"\01??_7error_category@std@@6B@" = unnamed_addr alias getelementptr inbounds ([7 x i8*], [7 x i8*]* @1, i32 0, i32 1)
@"\01??_7_Iostream_error_category@std@@6B@" = unnamed_addr alias getelementptr inbounds ([7 x i8*], [7 x i8*]* @2, i32 0, i32 1)
@"\01??_7_System_error_category@std@@6B@" = unnamed_addr alias getelementptr inbounds ([7 x i8*], [7 x i8*]* @3, i32 0, i32 1)

define i32 @"\01?$$MySetJmp@@YAHXZ"() #0 {
  %a = alloca i32, align 4
  %1 = call i32 (i8*, i32, ...)* @_setjmp3(i8* bitcast ([16 x i32]* @"\01?$$myJmpbuf@@3PAHA" to i8*), i32 0) #1
  store i32 %1, i32* %a, align 4
  ret i32 1
}

; Function Attrs: returns_twice
declare i32 @_setjmp3(i8*, i32, ...) #1

define void @"\01?$$MyLongJmp@@YAXXZ"() #0 {
  %1 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @"\01??_C@_0BJ@DMDFJPHM@?$CK?$CK?$CK?$CK?$CK?$CK?$CK?$CKlongJmp?$CK?$CK?$CK?$CK?$CK?$CK?$CK?$CK?$CK?$AA@", i32 0, i32 0))
  call void @longjmp(i32* getelementptr inbounds ([16 x i32], [16 x i32]* @"\01?$$myJmpbuf@@3PAHA", i32 0, i32 0), i32 1) #11
  unreachable
                                                  ; No predecessors!
  ret void
}

declare i32 @printf(i8*, ...) #0

; Function Attrs: noreturn
declare void @longjmp(i32*, i32) #2

define void @"\01?printPathTime@@YAXXZ"() #0 {
  %1 = call x86_stdcallcc i32 @"\01_GetTickCount@0"()
  %2 = load i32, i32* @"\01?start_time@@3KA", align 4
  %3 = sub i32 %1, %2
  %4 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([25 x i8], [25 x i8]* @"\01??_C@_0BJ@DOBLNIJM@This?5path?5cost?5?3?5?$CFlu?5ms?6?$AA@", i32 0, i32 0), i32 %3)
  %5 = call x86_stdcallcc i32 @"\01_GetTickCount@0"()
  store i32 %5, i32* @"\01?start_time@@3KA", align 4
  ret void
}

declare dllimport x86_stdcallcc i32 @"\01_GetTickCount@0"() #0

; Function Attrs: nounwind
define x86_stdcallcc i32 @"\01?ClientThread@@YGKPAX@Z"(i8* %lpParam) #3 {
  %1 = alloca i32, align 4
  %2 = alloca i8*, align 4
  store i8* %lpParam, i8** %2, align 4
  call void @llvm.trap()
  unreachable
                                                  ; No predecessors!
  %4 = load i32, i32* %1
  ret i32 %4
}

; Function Attrs: noreturn nounwind
declare void @llvm.trap() #4

; Function Attrs: nounwind
define i32 @"\01?Spe_InterlockedCompareExchange@@YAHPAJJJ@Z"(i32* %a, i32 %b, i32 %c) #3 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32*, align 4
  store i32 %c, i32* %1, align 4
  store i32 %b, i32* %2, align 4
  store i32* %a, i32** %3, align 4
  %4 = load i32, i32* %2, align 4
  %5 = load i32, i32* %1, align 4
  %6 = load i32*, i32** %3, align 4
  %7 = cmpxchg volatile i32* %6, i32 %5, i32 %4 seq_cst seq_cst
  %8 = extractvalue { i32, i1 } %7, 0
  ret i32 %8
}

define void @"\01?geterror@@YAXXZ"() #0 {
  %x = alloca i32, align 4
  %buf = alloca i8*, align 4
  %1 = call x86_stdcallcc i32 @"\01_GetLastError@0"()
  store i32 %1, i32* %x, align 4
  %2 = bitcast i8** %buf to i8*
  %3 = call x86_stdcallcc i32 @"\01_GetLastError@0"()
  %4 = call x86_stdcallcc i32 @"\01_FormatMessageA@28"(i32 5119, i8* null, i32 %3, i32 1024, i8* %2, i32 0, i8** null)
  %5 = icmp ne i32 %4, 0
  br i1 %5, label %6, label %14

; <label>:6                                       ; preds = %0
  %7 = load i8*, i8** %buf, align 4
  %8 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @"\01??_C@_03OFAPEBGM@?$CFs?6?$AA@", i32 0, i32 0), i8* %7)
  %9 = call %struct._iobuf* @__iob_func()
  %10 = getelementptr inbounds %struct._iobuf, %struct._iobuf* %9, i32 2
  %11 = call i32 @fflush(%struct._iobuf* %10)
  %12 = load i8*, i8** %buf, align 4
  %13 = call x86_stdcallcc i8* @"\01_LocalFree@4"(i8* %12)
  br label %14

; <label>:14                                      ; preds = %6, %0
  ret void
}

declare dllimport x86_stdcallcc i32 @"\01_GetLastError@0"() #0

declare dllimport x86_stdcallcc i32 @"\01_FormatMessageA@28"(i32, i8*, i32, i32, i8*, i32, i8**) #0

declare i32 @fflush(%struct._iobuf*) #0

declare %struct._iobuf* @__iob_func() #0

declare dllimport x86_stdcallcc i8* @"\01_LocalFree@4"(i8*) #0

define void @"\01?Spe_GetSystemInfo@@YAXPAUSPE_SYSTEM_INFO@@@Z"(%struct.SPE_SYSTEM_INFO* %x) #0 {
  %1 = alloca %struct.SPE_SYSTEM_INFO*, align 4
  %sys_info = alloca %struct._SYSTEM_INFO, align 4
  store %struct.SPE_SYSTEM_INFO* %x, %struct.SPE_SYSTEM_INFO** %1, align 4
  call x86_stdcallcc void @"\01_GetSystemInfo@4"(%struct._SYSTEM_INFO* %sys_info)
  %2 = load %struct.SPE_SYSTEM_INFO*, %struct.SPE_SYSTEM_INFO** %1, align 4
  %3 = getelementptr inbounds %struct.SPE_SYSTEM_INFO, %struct.SPE_SYSTEM_INFO* %2, i32 0, i32 0
  store i32 9, i32* %3, align 4
  %4 = getelementptr inbounds %struct._SYSTEM_INFO, %struct._SYSTEM_INFO* %sys_info, i32 0, i32 1
  %5 = load i32, i32* %4, align 4
  %6 = load %struct.SPE_SYSTEM_INFO*, %struct.SPE_SYSTEM_INFO** %1, align 4
  %7 = getelementptr inbounds %struct.SPE_SYSTEM_INFO, %struct.SPE_SYSTEM_INFO* %6, i32 0, i32 1
  store i32 %5, i32* %7, align 4
  %8 = getelementptr inbounds %struct._SYSTEM_INFO, %struct._SYSTEM_INFO* %sys_info, i32 0, i32 2
  %9 = load i8*, i8** %8, align 4
  %10 = load %struct.SPE_SYSTEM_INFO*, %struct.SPE_SYSTEM_INFO** %1, align 4
  %11 = getelementptr inbounds %struct.SPE_SYSTEM_INFO, %struct.SPE_SYSTEM_INFO* %10, i32 0, i32 2
  store i8* %9, i8** %11, align 4
  %12 = getelementptr inbounds %struct._SYSTEM_INFO, %struct._SYSTEM_INFO* %sys_info, i32 0, i32 3
  %13 = load i8*, i8** %12, align 4
  %14 = load %struct.SPE_SYSTEM_INFO*, %struct.SPE_SYSTEM_INFO** %1, align 4
  %15 = getelementptr inbounds %struct.SPE_SYSTEM_INFO, %struct.SPE_SYSTEM_INFO* %14, i32 0, i32 3
  store i8* %13, i8** %15, align 4
  %16 = getelementptr inbounds %struct._SYSTEM_INFO, %struct._SYSTEM_INFO* %sys_info, i32 0, i32 4
  %17 = load i32, i32* %16, align 4
  %18 = load %struct.SPE_SYSTEM_INFO*, %struct.SPE_SYSTEM_INFO** %1, align 4
  %19 = getelementptr inbounds %struct.SPE_SYSTEM_INFO, %struct.SPE_SYSTEM_INFO* %18, i32 0, i32 4
  store i32 %17, i32* %19, align 4
  %20 = getelementptr inbounds %struct._SYSTEM_INFO, %struct._SYSTEM_INFO* %sys_info, i32 0, i32 5
  %21 = load i32, i32* %20, align 4
  %22 = load %struct.SPE_SYSTEM_INFO*, %struct.SPE_SYSTEM_INFO** %1, align 4
  %23 = getelementptr inbounds %struct.SPE_SYSTEM_INFO, %struct.SPE_SYSTEM_INFO* %22, i32 0, i32 5
  store i32 %21, i32* %23, align 4
  %24 = getelementptr inbounds %struct._SYSTEM_INFO, %struct._SYSTEM_INFO* %sys_info, i32 0, i32 6
  %25 = load i32, i32* %24, align 4
  %26 = load %struct.SPE_SYSTEM_INFO*, %struct.SPE_SYSTEM_INFO** %1, align 4
  %27 = getelementptr inbounds %struct.SPE_SYSTEM_INFO, %struct.SPE_SYSTEM_INFO* %26, i32 0, i32 6
  store i32 %25, i32* %27, align 4
  %28 = getelementptr inbounds %struct._SYSTEM_INFO, %struct._SYSTEM_INFO* %sys_info, i32 0, i32 7
  %29 = load i32, i32* %28, align 4
  %30 = load %struct.SPE_SYSTEM_INFO*, %struct.SPE_SYSTEM_INFO** %1, align 4
  %31 = getelementptr inbounds %struct.SPE_SYSTEM_INFO, %struct.SPE_SYSTEM_INFO* %30, i32 0, i32 7
  store i32 %29, i32* %31, align 4
  %32 = getelementptr inbounds %struct._SYSTEM_INFO, %struct._SYSTEM_INFO* %sys_info, i32 0, i32 8
  %33 = load i16, i16* %32, align 2
  %34 = load %struct.SPE_SYSTEM_INFO*, %struct.SPE_SYSTEM_INFO** %1, align 4
  %35 = getelementptr inbounds %struct.SPE_SYSTEM_INFO, %struct.SPE_SYSTEM_INFO* %34, i32 0, i32 8
  store i16 %33, i16* %35, align 2
  %36 = getelementptr inbounds %struct._SYSTEM_INFO, %struct._SYSTEM_INFO* %sys_info, i32 0, i32 9
  %37 = load i16, i16* %36, align 2
  %38 = load %struct.SPE_SYSTEM_INFO*, %struct.SPE_SYSTEM_INFO** %1, align 4
  %39 = getelementptr inbounds %struct.SPE_SYSTEM_INFO, %struct.SPE_SYSTEM_INFO* %38, i32 0, i32 9
  store i16 %37, i16* %39, align 2
  ret void
}

declare dllimport x86_stdcallcc void @"\01_GetSystemInfo@4"(%struct._SYSTEM_INFO*) #0

define void @"\01?Spe_GetVersionExA@@YAXPAU_OSVERSIONINFOA@@@Z"(%struct._OSVERSIONINFOA* %b) #0 {
  %1 = alloca %struct._OSVERSIONINFOA*, align 4
  %x = alloca %struct._OSVERSIONINFOA, align 4
  store %struct._OSVERSIONINFOA* %b, %struct._OSVERSIONINFOA** %1, align 4
  %2 = call x86_stdcallcc i32 @"\01_GetVersionExA@4"(%struct._OSVERSIONINFOA* %x)
  %3 = getelementptr inbounds %struct._OSVERSIONINFOA, %struct._OSVERSIONINFOA* %x, i32 0, i32 1
  %4 = load i32, i32* %3, align 4
  %5 = load %struct._OSVERSIONINFOA*, %struct._OSVERSIONINFOA** %1, align 4
  %6 = getelementptr inbounds %struct._OSVERSIONINFOA, %struct._OSVERSIONINFOA* %5, i32 0, i32 1
  store i32 %4, i32* %6, align 4
  %7 = getelementptr inbounds %struct._OSVERSIONINFOA, %struct._OSVERSIONINFOA* %x, i32 0, i32 3
  %8 = load i32, i32* %7, align 4
  %9 = load %struct._OSVERSIONINFOA*, %struct._OSVERSIONINFOA** %1, align 4
  %10 = getelementptr inbounds %struct._OSVERSIONINFOA, %struct._OSVERSIONINFOA* %9, i32 0, i32 3
  store i32 %8, i32* %10, align 4
  %11 = getelementptr inbounds %struct._OSVERSIONINFOA, %struct._OSVERSIONINFOA* %x, i32 0, i32 2
  %12 = load i32, i32* %11, align 4
  %13 = load %struct._OSVERSIONINFOA*, %struct._OSVERSIONINFOA** %1, align 4
  %14 = getelementptr inbounds %struct._OSVERSIONINFOA, %struct._OSVERSIONINFOA* %13, i32 0, i32 2
  store i32 %12, i32* %14, align 4
  %15 = getelementptr inbounds %struct._OSVERSIONINFOA, %struct._OSVERSIONINFOA* %x, i32 0, i32 0
  %16 = load i32, i32* %15, align 4
  %17 = load %struct._OSVERSIONINFOA*, %struct._OSVERSIONINFOA** %1, align 4
  %18 = getelementptr inbounds %struct._OSVERSIONINFOA, %struct._OSVERSIONINFOA* %17, i32 0, i32 0
  store i32 %16, i32* %18, align 4
  %19 = getelementptr inbounds %struct._OSVERSIONINFOA, %struct._OSVERSIONINFOA* %x, i32 0, i32 4
  %20 = load i32, i32* %19, align 4
  %21 = load %struct._OSVERSIONINFOA*, %struct._OSVERSIONINFOA** %1, align 4
  %22 = getelementptr inbounds %struct._OSVERSIONINFOA, %struct._OSVERSIONINFOA* %21, i32 0, i32 4
  store i32 %20, i32* %22, align 4
  ret void
}

declare dllimport x86_stdcallcc i32 @"\01_GetVersionExA@4"(%struct._OSVERSIONINFOA*) #0

define void @"\01?ex@@YAXXZ"() #0 {
  %px = alloca i32*, align 4
  %c = alloca [10 x i8], align 1
  %b = alloca [10 x i8], align 1
  %x = alloca i32, align 4
  %lp = alloca i32*, align 4
  %fp = alloca %struct._iobuf*, align 4
  %Handle = alloca i8*, align 4
  %OSSemaphore = alloca i8*, align 4
  %cs = alloca %struct._RTL_CRITICAL_SECTION, align 4
  %Timecaps = alloca %struct.timecaps_tag, align 1
  %OS_Printf_marker = alloca i8*, align 4
  %timeaaa = alloca i64, align 8
  %a = alloca %struct._RTL_CRITICAL_SECTION, align 4
  %aaa = alloca i32*, align 4
  %xxxx = alloca i8, align 1
  %x1 = alloca i32, align 4
  %x2 = alloca i32, align 4
  %tepppppa = alloca i8*, align 4
  %1 = call i32 @_beginthreadex(i8* null, i32 1, i32 (i8*)* null, i8* null, i32 1, i32* null)
  call void @_endthreadex(i32 1)
  %2 = call i32 @strncmp(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @"\01??_C@_04JIHMPGLA@asdf?$AA@", i32 0, i32 0), i8* getelementptr inbounds ([4 x i8], [4 x i8]* @"\01??_C@_03BAOKKJLE@afd?$AA@", i32 0, i32 0), i32 13)
  call void @_ReadWriteBarrier()
  %3 = call noalias i8* @malloc(i32 8)
  %4 = bitcast i8* %3 to i32*
  store i32* %4, i32** %px, align 4
  %5 = load i32*, i32** %px, align 4
  %6 = bitcast i32* %5 to i8*
  %7 = call noalias i8* @realloc(i8* %6, i32 1)
  %8 = load i32*, i32** %px, align 4
  %9 = bitcast i32* %8 to i8*
  call void @llvm.memset.p0i8.i32(i8* %9, i8 2, i32 1, i32 4, i1 false)
  %10 = load i32*, i32** %px, align 4
  %11 = bitcast i32* %10 to i8*
  %12 = load i32*, i32** %px, align 4
  %13 = bitcast i32* %12 to i8*
  call void @llvm.memmove.p0i8.p0i8.i32(i8* %11, i8* %13, i32 1, i32 4, i1 false)
  %14 = load i32*, i32** %px, align 4
  %15 = bitcast i32* %14 to i8*
  call void @free(i8* %15)
  %16 = bitcast [10 x i8]* %c to i8*
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %16, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"\01?c@?1??ex@@YAXXZ@3PADA", i32 0, i32 0), i32 10, i32 1, i1 false)
  %17 = bitcast [10 x i8]* %b to i8*
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %17, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @"\01?b@?1??ex@@YAXXZ@3PADA", i32 0, i32 0), i32 10, i32 1, i1 false)
  %18 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %19 = call i32 @strlen(i8* %18)
  store i32 %19, i32* %x, align 4
  %20 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %21 = getelementptr inbounds [10 x i8], [10 x i8]* %b, i32 0, i32 0
  %22 = call i32 @strcmp(i8* %21, i8* %20)
  store i32 %22, i32* %x, align 4
  %23 = getelementptr inbounds [10 x i8], [10 x i8]* %b, i32 0, i32 0
  %24 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %25 = call i8* @strcpy(i8* %24, i8* %23)
  %26 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %27 = getelementptr inbounds [10 x i8], [10 x i8]* %b, i32 0, i32 0
  %28 = call i8* @strcat(i8* %27, i8* %26)
  %29 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %30 = getelementptr inbounds [10 x i8], [10 x i8]* %b, i32 0, i32 0
  %31 = call i8* @strncat(i8* %30, i8* %29, i32 1)
  %32 = load i32, i32* %x, align 4
  %33 = getelementptr inbounds [10 x i8], [10 x i8]* %b, i32 0, i32 0
  %34 = call i32 (i8*, i8*, ...)* @sprintf(i8* %33, i8* getelementptr inbounds ([2 x i8], [2 x i8]* @"\01??_C@_01MCMALHOG@a?$AA@", i32 0, i32 0), i32 %32)
  %35 = call i32 @system(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @"\01??_C@_0N@MMJFHEIA@IRResult?4exe?$AA@", i32 0, i32 0))
  call void @"\01?GetExternalVars@@YAXPAH0PADP6AXXZ1@Z"(i32* null, i32* null, i8* null, void ()* null, i8* null)
  %36 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 1
  %37 = load i8, i8* %36, align 1
  %38 = sext i8 %37 to i32
  %39 = call i32 @isdigit(i32 %38)
  %40 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 1
  %41 = load i8, i8* %40, align 1
  %42 = sext i8 %41 to i32
  %43 = call i32 @isupper(i32 %42)
  %44 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 1
  %45 = load i8, i8* %44, align 1
  %46 = sext i8 %45 to i32
  %47 = call i32 @tolower(i32 %46)
  %48 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 1
  %49 = load i8, i8* %48, align 1
  %50 = sext i8 %49 to i32
  %51 = call i32 @islower(i32 %50)
  %52 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 1
  %53 = load i8, i8* %52, align 1
  %54 = sext i8 %53 to i32
  %55 = call i32 @toupper(i32 %54)
  %56 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 1
  %57 = load i8, i8* %56, align 1
  %58 = sext i8 %57 to i32
  %59 = call i32 @isspace(i32 %58)
  %60 = call i32 @isatty(i32 0)
  %61 = call i32 @isalpha(i32 0)
  store i32* null, i32** %lp, align 4
  %62 = load i32*, i32** %lp, align 4
  %63 = cmpxchg volatile i32* %62, i32 0, i32 0 seq_cst seq_cst
  %64 = extractvalue { i32, i1 } %63, 0
  %65 = call i8* @strrchr(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @"\01??_C@_03MNDEGCIG@xxx?$AA@", i32 0, i32 0), i32 38)
  %66 = call i32 @strcspn(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @"\01??_C@_01FJMABOPO@x?$AA@", i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @"\01??_C@_01FJMABOPO@x?$AA@", i32 0, i32 0))
  %67 = call i32 @strspn(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @"\01??_C@_01FJMABOPO@x?$AA@", i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @"\01??_C@_01FJMABOPO@x?$AA@", i32 0, i32 0))
  %68 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %69 = call i32 @atoi(i8* %68)
  %70 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %71 = call double @atof(i8* %70)
  %72 = call double @floor(double 1.000000e+00) #12
  %73 = call double @sqrt(double 5.000000e+00) #5
  %74 = call double @pow(double 1.000000e+00, double 1.000000e+00) #5
  %75 = call double @exp(double 2.000000e+00) #5
  %76 = call double @sin(double 1.000000e+00) #5
  %77 = call double @asin(double 1.000000e+00) #5
  %78 = call double @tan(double 1.000000e+00) #5
  %79 = call double @atan(double 1.000000e+00) #5
  %80 = call double @atan2(double 1.000000e+00, double 1.000000e+00) #5
  %81 = call double @tanh(double 1.000000e+00) #5
  %82 = call double @cos(double 1.000000e+00) #5
  %83 = call double @acos(double 1.000000e+00) #5
  %84 = call i32 @abs(i32 1) #12
  %85 = call double @fabs(double 1.000000e+00) #12
  %86 = call double @fmod(double 1.000000e+00, double 1.000000e+00) #5
  %87 = call double @log(double 1.000000e+00) #5
  %88 = call double @log10(double 1.000000e+00) #5
  %89 = call noalias i8* @calloc(i32 1, i32 1)
  %90 = call i32 @clock()
  %91 = call i64 @"\01?time@@YA_JPA_J@Z"(i64* null)
  call void @srand(i32 0)
  %92 = call i32 @rand()
  %93 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %94 = call i8* @getenv(i8* %93)
  %95 = call %struct._iobuf* @fopen(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @"\01??_C@_05KAGKJOA@1?4txt?$AA@", i32 0, i32 0), i8* getelementptr inbounds ([2 x i8], [2 x i8]* @"\01??_C@_01KDCPPGHE@r?$AA@", i32 0, i32 0))
  store %struct._iobuf* %95, %struct._iobuf** %fp, align 4
  %96 = load %struct._iobuf*, %struct._iobuf** %fp, align 4
  %97 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %98 = call i32 @fread(i8* %97, i32 1, i32 1, %struct._iobuf* %96)
  %99 = load i32, i32* %x, align 4
  %100 = load %struct._iobuf*, %struct._iobuf** %fp, align 4
  %101 = call i32 (%struct._iobuf*, i8*, ...)* @fprintf(%struct._iobuf* %100, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @"\01??_C@_02DPKJAMEF@?$CFd?$AA@", i32 0, i32 0), i32 %99)
  %102 = load i32, i32* %x, align 4
  %103 = call %struct._iobuf* @__iob_func()
  %104 = getelementptr inbounds %struct._iobuf, %struct._iobuf* %103, i32 0
  %105 = call i32 (%struct._iobuf*, i8*, ...)* @fprintf(%struct._iobuf* %104, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @"\01??_C@_02DPKJAMEF@?$CFd?$AA@", i32 0, i32 0), i32 %102)
  %106 = load i32, i32* %x, align 4
  %107 = call %struct._iobuf* @__iob_func()
  %108 = getelementptr inbounds %struct._iobuf, %struct._iobuf* %107, i32 1
  %109 = call i32 (%struct._iobuf*, i8*, ...)* @fprintf(%struct._iobuf* %108, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @"\01??_C@_02DPKJAMEF@?$CFd?$AA@", i32 0, i32 0), i32 %106)
  %110 = load i32, i32* %x, align 4
  %111 = call %struct._iobuf* @__iob_func()
  %112 = getelementptr inbounds %struct._iobuf, %struct._iobuf* %111, i32 2
  %113 = call i32 (%struct._iobuf*, i8*, ...)* @fprintf(%struct._iobuf* %112, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @"\01??_C@_02DPKJAMEF@?$CFd?$AA@", i32 0, i32 0), i32 %110)
  %114 = load %struct._iobuf*, %struct._iobuf** %fp, align 4
  %115 = call i32 @fileno(%struct._iobuf* %114)
  %116 = load %struct._iobuf*, %struct._iobuf** %fp, align 4
  %117 = call i32 (%struct._iobuf*, i8*, ...)* @fscanf(%struct._iobuf* %116, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @"\01??_C@_02DPKJAMEF@?$CFd?$AA@", i32 0, i32 0), i32* %x)
  %118 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %119 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %120 = call i32 (i8*, i8*, ...)* @sscanf(i8* %119, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @"\01??_C@_02DKCKIIND@?$CFs?$AA@", i32 0, i32 0), i8* %118)
  %121 = load %struct._iobuf*, %struct._iobuf** %fp, align 4
  %122 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %123 = call i32 @fwrite(i8* %122, i32 1, i32 1, %struct._iobuf* %121)
  %124 = load %struct._iobuf*, %struct._iobuf** %fp, align 4
  %125 = call i32 @fseek(%struct._iobuf* %124, i32 1, i32 0)
  %126 = load %struct._iobuf*, %struct._iobuf** %fp, align 4
  %127 = call i32 @feof(%struct._iobuf* %126)
  %128 = call i8* @strtok(i8* null, i8* getelementptr inbounds ([5 x i8], [5 x i8]* @"\01??_C@_04HLKDGPAO@sdfs?$AA@", i32 0, i32 0))
  %129 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %130 = call i8* @itoa(i32 1, i8* %129, i32 10)
  %131 = load %struct._iobuf*, %struct._iobuf** %fp, align 4
  call void @rewind(%struct._iobuf* %131)
  %132 = load %struct._iobuf*, %struct._iobuf** %fp, align 4
  %133 = call i32 @fflush(%struct._iobuf* %132)
  %134 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %135 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %136 = call i32 @rename(i8* %135, i8* %134)
  %137 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %138 = call i32 @_unlink(i8* %137)
  %139 = call i32 @read(i32 0, i8* null, i32 0)
  %140 = call i8* @tmpnam(i8* null)
  %141 = call i32 @write(i32 0, i8* null, i32 0)
  %142 = call i32 (i8*, i32, ...)* @_setjmp3(i8* bitcast ([16 x i32]* @"\01?j_buf@@3PAHA" to i8*), i32 0) #1
  %143 = call i32 @memcmp(i8* null, i8* null, i32 10)
  call void @"\01?AddEdge@@YAXPBDHH@Z"(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @"\01??_C@_00CNPNBAHC@?$AA@", i32 0, i32 0), i32 0, i32 0)
  call void @"\01?Draw@@YAXXZ"()
  call void @"\01?UpdateCurrentNode@@YAXXZ"()
  call void @"\01?SaveCurrentNode@@YAXXZ"()
  %144 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %145 = load i8, i8* %144, align 1
  %146 = sext i8 %145 to i32
  %147 = icmp ne i32 %146, -1
  br i1 %147, label %148, label %152

; <label>:148                                     ; preds = %0
  %149 = load %struct._iobuf*, %struct._iobuf** %fp, align 4
  %150 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %151 = call i32 @fputs(i8* %150, %struct._iobuf* %149)
  br label %152

; <label>:152                                     ; preds = %148, %0
  %153 = load %struct._iobuf*, %struct._iobuf** %fp, align 4
  %154 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 1
  %155 = load i8, i8* %154, align 1
  %156 = sext i8 %155 to i32
  %157 = call i32 @fputc(i32 %156, %struct._iobuf* %153)
  %158 = load %struct._iobuf*, %struct._iobuf** %fp, align 4
  %159 = call i32 @fgetc(%struct._iobuf* %158)
  %160 = load %struct._iobuf*, %struct._iobuf** %fp, align 4
  %161 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %162 = call i8* @fgets(i8* %161, i32 10, %struct._iobuf* %160)
  %163 = call %struct._iobuf* @tmpfile()
  %164 = call i32 @getchar()
  %165 = call i32 @putchar(i32 97)
  %166 = load %struct._iobuf*, %struct._iobuf** %fp, align 4
  %167 = call i32 @fclose(%struct._iobuf* %166)
  %168 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %169 = call i32 (i8*, i32, ...)* @open(i8* %168, i32 32768)
  %170 = call i32 @_close(i32 1)
  %171 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  call void @perror(i8* %171)
  %172 = call i32* @_errno()
  %173 = load i32, i32* %172, align 4
  %174 = call i8* @strerror(i32 %173)
  %175 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %176 = call i32 @remove(i8* %175)
  %177 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %178 = call i32 @_read(i32 1, i8* %177, i32 1)
  call x86_stdcallcc void @"\01_Sleep@4"(i32 1)
  %179 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %180 = load %struct._iobuf*, %struct._iobuf** %fp, align 4
  call void @setbuf(%struct._iobuf* %180, i8* %179)
  %181 = call %struct.tm* @"\01?localtime@@YAPAUtm@@PB_J@Z"(i64* %timeaaa)
  call x86_stdcallcc void @"\01_DeleteCriticalSection@4"(%struct._RTL_CRITICAL_SECTION* %a)
  %182 = load i32*, i32** %aaa, align 4
  %183 = call i32 @"\01?Spe_InterlockedCompareExchange@@YAHPAJJJ@Z"(i32* %182, i32 0, i32 0)
  %184 = load i8, i8* %xxxx, align 1
  %185 = zext i8 %184 to i16
  %186 = call zeroext i16 @_byteswap_ushort(i16 zeroext %185)
  call void @_ReadWriteBarrier()
  %187 = load i32, i32* %x1, align 4
  %188 = call i32 @_byteswap_ulong(i32 %187)
  %189 = call i32 @_beginthreadex(i8* null, i32 0, i32 (i8*)* null, i8* null, i32 0, i32* null)
  call void @_endthreadex(i32 0)
  %190 = call i32 @strncmp(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @"\01??_C@_00CNPNBAHC@?$AA@", i32 0, i32 0), i8* getelementptr inbounds ([1 x i8], [1 x i8]* @"\01??_C@_00CNPNBAHC@?$AA@", i32 0, i32 0), i32 1)
  %191 = call x86_stdcallcc i32 @"\01_AreFileApisANSI@0"()
  %192 = call x86_stdcallcc i32 @"\01_CloseHandle@4"(i8* null)
  %193 = call x86_stdcallcc i8* @"\01_CreateFileA@28"(i8* null, i32 0, i32 0, %struct._SECURITY_ATTRIBUTES* null, i32 0, i32 0, i8* null)
  %194 = call x86_stdcallcc i8* @"\01_CreateFileW@28"(i16* null, i32 0, i32 0, %struct._SECURITY_ATTRIBUTES* null, i32 0, i32 0, i8* null)
  %195 = call x86_stdcallcc i8* @"\01_CreateFileMappingA@24"(i8* null, %struct._SECURITY_ATTRIBUTES* null, i32 0, i32 0, i32 0, i8* null)
  %196 = call x86_stdcallcc i8* @"\01_CreateFileMappingW@24"(i8* null, %struct._SECURITY_ATTRIBUTES* null, i32 0, i32 0, i32 0, i16* null)
  %197 = call x86_stdcallcc i8* @"\01_CreateMutexW@12"(%struct._SECURITY_ATTRIBUTES* null, i32 0, i16* null)
  %198 = call x86_stdcallcc i32 @"\01_DeleteFileA@4"(i8* null)
  %199 = call x86_stdcallcc i32 @"\01_DeleteFileW@4"(i16* null)
  %200 = call x86_stdcallcc i32 @"\01_FlushFileBuffers@4"(i8* null)
  %201 = call x86_stdcallcc i32 @"\01_FormatMessageA@28"(i32 0, i8* null, i32 0, i32 0, i8* null, i32 0, i8** null)
  %202 = call x86_stdcallcc i32 @"\01_FormatMessageW@28"(i32 0, i8* null, i32 0, i32 0, i16* null, i32 0, i8** null)
  %203 = call x86_stdcallcc i32 @"\01_FreeLibrary@4"(%struct.HINSTANCE__* null)
  %204 = call x86_stdcallcc i32 @"\01_GetCurrentProcessId@0"()
  %205 = call x86_stdcallcc i32 @"\01_GetDiskFreeSpaceA@20"(i8* null, i32* null, i32* null, i32* null, i32* null)
  %206 = call x86_stdcallcc i32 @"\01_GetDiskFreeSpaceW@20"(i16* null, i32* null, i32* null, i32* null, i32* null)
  %207 = call x86_stdcallcc i32 @"\01_GetFileAttributesA@4"(i8* null)
  %208 = call x86_stdcallcc i32 @"\01_GetFileAttributesW@4"(i16* null)
  %209 = load i32, i32* %x2, align 4
  %210 = call x86_stdcallcc i32 @"\01_GetFileAttributesExW@12"(i16* null, i32 %209, i8* null)
  %211 = call x86_stdcallcc i32 @"\01_GetFileSize@8"(i8* null, i32* null)
  %212 = call x86_stdcallcc i32 @"\01_GetFullPathNameA@16"(i8* null, i32 0, i8* null, i8** null)
  %213 = call x86_stdcallcc i32 @"\01_GetFullPathNameW@16"(i16* null, i32 0, i16* null, i16** null)
  %214 = call x86_stdcallcc i32 @"\01_GetLastError@0"()
  %215 = call x86_stdcallcc i32 ()* (%struct.HINSTANCE__*, i8*)* @"\01_GetProcAddress@8"(%struct.HINSTANCE__* null, i8* null)
  call x86_stdcallcc void @"\01_GetSystemInfo@4"(%struct._SYSTEM_INFO* null)
  call x86_stdcallcc void @"\01_GetSystemTime@4"(%struct._SYSTEMTIME* null)
  call x86_stdcallcc void @"\01_GetSystemTimeAsFileTime@4"(%struct._FILETIME* null)
  %216 = call x86_stdcallcc i32 @"\01_GetTempPathA@8"(i32 0, i8* null)
  %217 = call x86_stdcallcc i32 @"\01_GetTempPathW@8"(i32 0, i16* null)
  %218 = call x86_stdcallcc i32 @"\01_GetTickCount@0"()
  %219 = call x86_stdcallcc i32 @"\01_GetVersionExA@4"(%struct._OSVERSIONINFOA* null)
  %220 = call x86_stdcallcc i32 @"\01_GetVersionExW@4"(%struct._OSVERSIONINFOW* null)
  %221 = call x86_stdcallcc i8* @"\01_HeapAlloc@12"(i8* null, i32 0, i32 0)
  %222 = call x86_stdcallcc i8* @"\01_HeapCreate@12"(i32 0, i32 0, i32 0)
  %223 = call x86_stdcallcc i32 @"\01_HeapDestroy@4"(i8* null)
  %224 = call x86_stdcallcc i32 @"\01_HeapFree@12"(i8* null, i32 0, i8* null)
  %225 = call x86_stdcallcc i8* @"\01_HeapReAlloc@16"(i8* null, i32 0, i8* null, i32 0)
  %226 = call x86_stdcallcc i32 @"\01_HeapSize@12"(i8* null, i32 0, i8* null)
  %227 = call x86_stdcallcc i32 @"\01_HeapValidate@12"(i8* null, i32 0, i8* null)
  %228 = call x86_stdcallcc i32 @"\01_HeapCompact@8"(i8* null, i32 0)
  %229 = call x86_stdcallcc %struct.HINSTANCE__* @"\01_LoadLibraryA@4"(i8* null)
  %230 = call x86_stdcallcc %struct.HINSTANCE__* @"\01_LoadLibraryW@4"(i16* null)
  %231 = call x86_stdcallcc i8* @"\01_LocalFree@4"(i8* null)
  %232 = call x86_stdcallcc i32 @"\01_LockFile@20"(i8* null, i32 0, i32 0, i32 0, i32 0)
  %233 = call x86_stdcallcc i32 @"\01_LockFileEx@24"(i8* null, i32 0, i32 0, i32 0, i32 0, %struct._OVERLAPPED* null)
  %234 = call x86_stdcallcc i8* @"\01_MapViewOfFile@20"(i8* null, i32 0, i32 0, i32 0, i32 0)
  %235 = call x86_stdcallcc i32 @"\01_MultiByteToWideChar@24"(i32 0, i32 0, i8* null, i32 0, i16* null, i32 0)
  %236 = call x86_stdcallcc i32 @"\01_QueryPerformanceCounter@4"(%union._LARGE_INTEGER* null)
  %237 = call x86_stdcallcc i32 @"\01_ReadFile@20"(i8* null, i8* null, i32 0, i32* null, %struct._OVERLAPPED* null)
  %238 = call x86_stdcallcc i32 @"\01_SetEndOfFile@4"(i8* null)
  %239 = call x86_stdcallcc i32 @"\01_SetFilePointer@16"(i8* null, i32 0, i32* null, i32 0)
  call x86_stdcallcc void @"\01_Sleep@4"(i32 0)
  %240 = call x86_stdcallcc i32 @"\01_SystemTimeToFileTime@8"(%struct._SYSTEMTIME* null, %struct._FILETIME* null)
  %241 = call x86_stdcallcc i32 @"\01_UnlockFile@20"(i8* null, i32 0, i32 0, i32 0, i32 0)
  %242 = call x86_stdcallcc i32 @"\01_UnlockFileEx@20"(i8* null, i32 0, i32 0, i32 0, %struct._OVERLAPPED* null)
  %243 = call x86_stdcallcc i32 @"\01_UnmapViewOfFile@4"(i8* null)
  %244 = call x86_stdcallcc i32 @"\01_WideCharToMultiByte@32"(i32 0, i32 0, i16* null, i32 0, i8* null, i32 0, i8* null, i32* null)
  %245 = call x86_stdcallcc i32 @"\01_WriteFile@20"(i8* null, i8* null, i32 0, i32* null, %struct._OVERLAPPED* null)
  %246 = call x86_stdcallcc i32 @"\01_WaitForSingleObject@8"(i8* null, i32 0)
  %247 = call x86_stdcallcc i32 @"\01_WaitForSingleObjectEx@12"(i8* null, i32 0, i32 0)
  call x86_stdcallcc void @"\01_OutputDebugStringA@4"(i8* null)
  call x86_stdcallcc void @"\01_OutputDebugStringW@4"(i16* null)
  %248 = call x86_stdcallcc i8* @"\01_GetProcessHeap@0"()
  %249 = call x86_stdcallcc i32 @"\01_FlushViewOfFile@8"(i8* null, i32 0)
  call x86_stdcallcc void @"\01_EnterCriticalSection@4"(%struct._RTL_CRITICAL_SECTION* %cs)
  call x86_stdcallcc void @"\01_LeaveCriticalSection@4"(%struct._RTL_CRITICAL_SECTION* %cs)
  call x86_stdcallcc void @"\01_InitializeCriticalSection@4"(%struct._RTL_CRITICAL_SECTION* %cs)
  %250 = load i8*, i8** %OSSemaphore, align 4
  %251 = call x86_stdcallcc i32 @"\01_WaitForSingleObject@8"(i8* %250, i32 1)
  %252 = load i8*, i8** %OSSemaphore, align 4
  %253 = call x86_stdcallcc i32 @"\01_ReleaseSemaphore@12"(i8* %252, i32 1, i32* null)
  %254 = call x86_stdcallcc i8* @"\01_CreateSemaphoreA@16"(%struct._SECURITY_ATTRIBUTES* null, i32 1, i32 1, i8* null)
  %255 = load i8*, i8** %Handle, align 4
  %256 = call x86_stdcallcc i32 @"\01_SetThreadAffinityMask@8"(i8* %255, i32 1)
  %257 = call x86_stdcallcc i32 @"\01_GetCurrentThreadId@0"()
  %258 = call x86_stdcallcc i8* @"\01_CreateThread@24"(%struct._SECURITY_ATTRIBUTES* null, i32 0, i32 (i8*)* @"\01?ClientThread@@YGKPAX@Z", i8* null, i32 0, i32* null)
  %259 = load i8*, i8** %Handle, align 4
  %260 = call x86_stdcallcc i32 @"\01_SuspendThread@4"(i8* %259)
  %261 = load i8*, i8** %Handle, align 4
  %262 = call x86_stdcallcc i32 @"\01_ResumeThread@4"(i8* %261)
  %263 = load i8*, i8** %Handle, align 4
  %264 = call x86_stdcallcc i32 @"\01_SetThreadPriority@8"(i8* %263, i32 2)
  %265 = load i8*, i8** %Handle, align 4
  %266 = call x86_stdcallcc i32 @"\01_SetThreadPriorityBoost@8"(i8* %265, i32 1)
  %267 = load i8*, i8** %Handle, align 4
  %268 = call x86_stdcallcc i32 @"\01_CloseHandle@4"(i8* %267)
  %269 = call x86_stdcallcc i8* @"\01_CreateEventA@16"(%struct._SECURITY_ATTRIBUTES* null, i32 0, i32 0, i8* null)
  %270 = load i8*, i8** %Handle, align 4
  %271 = call x86_stdcallcc i32 @"\01_ResetEvent@4"(i8* %270)
  %272 = call x86_stdcallcc i32 @"\01_timeSetEvent@20"(i32 10, i32 1, void (i32, i32, i32, i32, i32)* null, i32 1, i32 1)
  %273 = call x86_stdcallcc i32 @"\01_timeKillEvent@4"(i32 0)
  %274 = call x86_stdcallcc i32 @"\01_timeEndPeriod@4"(i32 0)
  %275 = call x86_stdcallcc i32 @"\01_timeGetDevCaps@8"(%struct.timecaps_tag* %Timecaps, i32 0)
  %276 = call x86_stdcallcc i32 @"\01_timeBeginPeriod@4"(i32 0)
  %277 = bitcast [10 x i8]* %c to i8*
  %278 = getelementptr inbounds i8, i8* %277, i32 12
  store i8* %278, i8** %OS_Printf_marker, align 4
  store i8* null, i8** %OS_Printf_marker, align 4
  %279 = load i8*, i8** %Handle, align 4
  %280 = call x86_stdcallcc i32 @"\01_SetPriorityClass@8"(i8* %279, i32 2)
  %281 = call x86_stdcallcc i32 @"\01_GetTickCount@0"()
  %282 = load i8*, i8** %OS_Printf_marker, align 4
  %283 = getelementptr inbounds [10 x i8], [10 x i8]* %c, i32 0, i32 0
  %284 = call i32 @vprintf(i8* %283, i8* %282)
  call void @"\01?MyCreateThread@@YAXP6GIPAX@Z0H@Z"(i32 (i8*)* null, i8* null, i32 1)
  call void @"\01?MyPauseThread@@YAXXZ"()
  call void @"\01?MyWaitForObject@@YAXXZ"()
  call void @"\01?setNodeLength@@YAXH@Z"(i32 1)
  call void @"\01?$$Push@@YAXPAD0H@Z"(i8* getelementptr inbounds ([1 x i8], [1 x i8]* @"\01??_C@_00CNPNBAHC@?$AA@", i32 0, i32 0), i8* getelementptr inbounds ([1 x i8], [1 x i8]* @"\01??_C@_00CNPNBAHC@?$AA@", i32 0, i32 0), i32 1)
  call void @"\01?GetMsvVar@@YAXPAHP6AXXZP6A_NXZ@Z"(i32* null, void ()* null, i1 ()* null)
  call void @"\01?addStatement@@YAXH@Z"(i32 0)
  call void @"\01?GetModelVar@@YAXPAY0BIGKA@DPAHP6AXXZ@Z"([100000 x i8]* null, i32* null, void ()* null)
  call void @"\01?whileBegin@@YAXII@Z"(i32 0, i32 0)
  call void @"\01?whileEnd@@YAXII@Z"(i32 0, i32 0)
  call void @"\01?setModelOption@@YAX_N@Z"(i1 zeroext false)
  call void @"\01?Spe_GetSystemInfo@@YAXPAUSPE_SYSTEM_INFO@@@Z"(%struct.SPE_SYSTEM_INFO* null)
  call void @"\01?Spe_GetVersionExA@@YAXPAU_OSVERSIONINFOA@@@Z"(%struct._OSVERSIONINFOA* null)
  store i8* getelementptr inbounds ([1 x i8], [1 x i8]* @"\01??_C@_00CNPNBAHC@?$AA@", i32 0, i32 0), i8** %tepppppa, align 4
  %285 = load i8*, i8** %tepppppa, align 4
  %286 = call double @strtod(i8* %285, i8** null)
  call void @exit(i32 0) #11
  unreachable
                                                  ; No predecessors!
  ret void
}

declare i32 @_beginthreadex(i8*, i32, i32 (i8*)*, i8*, i32, i32*) #0

declare void @_endthreadex(i32) #0

declare i32 @strncmp(i8*, i8*, i32) #0

declare void @_ReadWriteBarrier() #0

declare noalias i8* @malloc(i32) #0

declare noalias i8* @realloc(i8*, i32) #0

; Function Attrs: nounwind
declare void @llvm.memset.p0i8.i32(i8* nocapture, i8, i32, i32, i1) #5

; Function Attrs: nounwind
declare void @llvm.memmove.p0i8.p0i8.i32(i8* nocapture, i8* nocapture readonly, i32, i32, i1) #5

declare void @free(i8*) #0

; Function Attrs: nounwind
declare void @llvm.memcpy.p0i8.p0i8.i32(i8* nocapture, i8* nocapture readonly, i32, i32, i1) #5

declare i32 @strlen(i8*) #0

declare i32 @strcmp(i8*, i8*) #0

declare i8* @strcpy(i8*, i8*) #0

declare i8* @strcat(i8*, i8*) #0

declare i8* @strncat(i8*, i8*, i32) #0

declare i32 @sprintf(i8*, i8*, ...) #0

declare i32 @system(i8*) #0

declare void @"\01?GetExternalVars@@YAXPAH0PADP6AXXZ1@Z"(i32*, i32*, i8*, void ()*, i8*) #0

declare i32 @isdigit(i32) #0

declare i32 @isupper(i32) #0

declare i32 @tolower(i32) #0

declare i32 @islower(i32) #0

declare i32 @toupper(i32) #0

declare i32 @isspace(i32) #0

declare i32 @isatty(i32) #0

declare i32 @isalpha(i32) #0

declare i8* @strrchr(i8*, i32) #0

declare i32 @strcspn(i8*, i8*) #0

declare i32 @strspn(i8*, i8*) #0

declare i32 @atoi(i8*) #0

declare double @atof(i8*) #0

; Function Attrs: nounwind readnone
declare double @floor(double) #6

; Function Attrs: nounwind
declare double @sqrt(double) #3

; Function Attrs: nounwind
declare double @pow(double, double) #3

; Function Attrs: nounwind
declare double @exp(double) #3

; Function Attrs: nounwind
declare double @sin(double) #3

; Function Attrs: nounwind
declare double @asin(double) #3

; Function Attrs: nounwind
declare double @tan(double) #3

; Function Attrs: nounwind
declare double @atan(double) #3

; Function Attrs: nounwind
declare double @atan2(double, double) #3

; Function Attrs: nounwind
declare double @tanh(double) #3

; Function Attrs: nounwind
declare double @cos(double) #3

; Function Attrs: nounwind
declare double @acos(double) #3

; Function Attrs: nounwind readnone
declare i32 @abs(i32) #6

; Function Attrs: nounwind readnone
declare double @fabs(double) #6

; Function Attrs: nounwind
declare double @fmod(double, double) #3

; Function Attrs: nounwind
declare double @log(double) #3

; Function Attrs: nounwind
declare double @log10(double) #3

declare noalias i8* @calloc(i32, i32) #0

declare i32 @clock() #0

; Function Attrs: inlinehint
define internal i64 @"\01?time@@YA_JPA_J@Z"(i64* %_Time) #7 {
  %1 = alloca i64*, align 4
  store i64* %_Time, i64** %1, align 4
  %2 = load i64*, i64** %1, align 4
  %3 = call i64 @_time64(i64* %2)
  ret i64 %3
}

declare void @srand(i32) #0

declare i32 @rand() #0

declare i8* @getenv(i8*) #0

declare %struct._iobuf* @fopen(i8*, i8*) #0

declare i32 @fread(i8*, i32, i32, %struct._iobuf*) #0

declare i32 @fprintf(%struct._iobuf*, i8*, ...) #0

declare i32 @fileno(%struct._iobuf*) #0

declare i32 @fscanf(%struct._iobuf*, i8*, ...) #0

declare i32 @sscanf(i8*, i8*, ...) #0

declare i32 @fwrite(i8*, i32, i32, %struct._iobuf*) #0

declare i32 @fseek(%struct._iobuf*, i32, i32) #0

declare i32 @feof(%struct._iobuf*) #0

declare i8* @strtok(i8*, i8*) #0

declare i8* @itoa(i32, i8*, i32) #0

declare void @rewind(%struct._iobuf*) #0

declare i32 @rename(i8*, i8*) #0

declare i32 @_unlink(i8*) #0

declare i32 @read(i32, i8*, i32) #0

declare i8* @tmpnam(i8*) #0

declare i32 @write(i32, i8*, i32) #0

declare i32 @memcmp(i8*, i8*, i32) #0

declare void @"\01?AddEdge@@YAXPBDHH@Z"(i8*, i32, i32) #0

declare void @"\01?Draw@@YAXXZ"() #0

declare void @"\01?UpdateCurrentNode@@YAXXZ"() #0

declare void @"\01?SaveCurrentNode@@YAXXZ"() #0

declare i32 @fputs(i8*, %struct._iobuf*) #0

declare i32 @fputc(i32, %struct._iobuf*) #0

declare i32 @fgetc(%struct._iobuf*) #0

declare i8* @fgets(i8*, i32, %struct._iobuf*) #0

declare %struct._iobuf* @tmpfile() #0

declare i32 @getchar() #0

declare i32 @putchar(i32) #0

declare i32 @fclose(%struct._iobuf*) #0

declare i32 @open(i8*, i32, ...) #0

declare i32 @_close(i32) #0

declare void @perror(i8*) #0

declare i8* @strerror(i32) #0

declare i32* @_errno() #0

declare i32 @remove(i8*) #0

declare i32 @_read(i32, i8*, i32) #0

declare dllimport x86_stdcallcc void @"\01_Sleep@4"(i32) #0

declare void @setbuf(%struct._iobuf*, i8*) #0

; Function Attrs: inlinehint
define internal %struct.tm* @"\01?localtime@@YAPAUtm@@PB_J@Z"(i64* %_Time) #7 {
  %1 = alloca i64*, align 4
  store i64* %_Time, i64** %1, align 4
  %2 = load i64*, i64** %1, align 4
  %3 = call %struct.tm* @_localtime64(i64* %2)
  ret %struct.tm* %3
}

declare dllimport x86_stdcallcc void @"\01_DeleteCriticalSection@4"(%struct._RTL_CRITICAL_SECTION*) #0

declare zeroext i16 @_byteswap_ushort(i16 zeroext) #0

declare i32 @_byteswap_ulong(i32) #0

declare dllimport x86_stdcallcc i32 @"\01_AreFileApisANSI@0"() #0

declare dllimport x86_stdcallcc i32 @"\01_CloseHandle@4"(i8*) #0

declare dllimport x86_stdcallcc i8* @"\01_CreateFileA@28"(i8*, i32, i32, %struct._SECURITY_ATTRIBUTES*, i32, i32, i8*) #0

declare dllimport x86_stdcallcc i8* @"\01_CreateFileW@28"(i16*, i32, i32, %struct._SECURITY_ATTRIBUTES*, i32, i32, i8*) #0

declare dllimport x86_stdcallcc i8* @"\01_CreateFileMappingA@24"(i8*, %struct._SECURITY_ATTRIBUTES*, i32, i32, i32, i8*) #0

declare dllimport x86_stdcallcc i8* @"\01_CreateFileMappingW@24"(i8*, %struct._SECURITY_ATTRIBUTES*, i32, i32, i32, i16*) #0

declare dllimport x86_stdcallcc i8* @"\01_CreateMutexW@12"(%struct._SECURITY_ATTRIBUTES*, i32, i16*) #0

declare dllimport x86_stdcallcc i32 @"\01_DeleteFileA@4"(i8*) #0

declare dllimport x86_stdcallcc i32 @"\01_DeleteFileW@4"(i16*) #0

declare dllimport x86_stdcallcc i32 @"\01_FlushFileBuffers@4"(i8*) #0

declare dllimport x86_stdcallcc i32 @"\01_FormatMessageW@28"(i32, i8*, i32, i32, i16*, i32, i8**) #0

declare dllimport x86_stdcallcc i32 @"\01_FreeLibrary@4"(%struct.HINSTANCE__*) #0

declare dllimport x86_stdcallcc i32 @"\01_GetCurrentProcessId@0"() #0

declare dllimport x86_stdcallcc i32 @"\01_GetDiskFreeSpaceA@20"(i8*, i32*, i32*, i32*, i32*) #0

declare dllimport x86_stdcallcc i32 @"\01_GetDiskFreeSpaceW@20"(i16*, i32*, i32*, i32*, i32*) #0

declare dllimport x86_stdcallcc i32 @"\01_GetFileAttributesA@4"(i8*) #0

declare dllimport x86_stdcallcc i32 @"\01_GetFileAttributesW@4"(i16*) #0

declare dllimport x86_stdcallcc i32 @"\01_GetFileAttributesExW@12"(i16*, i32, i8*) #0

declare dllimport x86_stdcallcc i32 @"\01_GetFileSize@8"(i8*, i32*) #0

declare dllimport x86_stdcallcc i32 @"\01_GetFullPathNameA@16"(i8*, i32, i8*, i8**) #0

declare dllimport x86_stdcallcc i32 @"\01_GetFullPathNameW@16"(i16*, i32, i16*, i16**) #0

declare dllimport x86_stdcallcc i32 ()* @"\01_GetProcAddress@8"(%struct.HINSTANCE__*, i8*) #0

declare dllimport x86_stdcallcc void @"\01_GetSystemTime@4"(%struct._SYSTEMTIME*) #0

declare dllimport x86_stdcallcc void @"\01_GetSystemTimeAsFileTime@4"(%struct._FILETIME*) #0

declare dllimport x86_stdcallcc i32 @"\01_GetTempPathA@8"(i32, i8*) #0

declare dllimport x86_stdcallcc i32 @"\01_GetTempPathW@8"(i32, i16*) #0

declare dllimport x86_stdcallcc i32 @"\01_GetVersionExW@4"(%struct._OSVERSIONINFOW*) #0

declare dllimport x86_stdcallcc i8* @"\01_HeapAlloc@12"(i8*, i32, i32) #0

declare dllimport x86_stdcallcc i8* @"\01_HeapCreate@12"(i32, i32, i32) #0

declare dllimport x86_stdcallcc i32 @"\01_HeapDestroy@4"(i8*) #0

declare dllimport x86_stdcallcc i32 @"\01_HeapFree@12"(i8*, i32, i8*) #0

declare dllimport x86_stdcallcc i8* @"\01_HeapReAlloc@16"(i8*, i32, i8*, i32) #0

declare dllimport x86_stdcallcc i32 @"\01_HeapSize@12"(i8*, i32, i8*) #0

declare dllimport x86_stdcallcc i32 @"\01_HeapValidate@12"(i8*, i32, i8*) #0

declare dllimport x86_stdcallcc i32 @"\01_HeapCompact@8"(i8*, i32) #0

declare dllimport x86_stdcallcc %struct.HINSTANCE__* @"\01_LoadLibraryA@4"(i8*) #0

declare dllimport x86_stdcallcc %struct.HINSTANCE__* @"\01_LoadLibraryW@4"(i16*) #0

declare dllimport x86_stdcallcc i32 @"\01_LockFile@20"(i8*, i32, i32, i32, i32) #0

declare dllimport x86_stdcallcc i32 @"\01_LockFileEx@24"(i8*, i32, i32, i32, i32, %struct._OVERLAPPED*) #0

declare dllimport x86_stdcallcc i8* @"\01_MapViewOfFile@20"(i8*, i32, i32, i32, i32) #0

declare dllimport x86_stdcallcc i32 @"\01_MultiByteToWideChar@24"(i32, i32, i8*, i32, i16*, i32) #0

declare dllimport x86_stdcallcc i32 @"\01_QueryPerformanceCounter@4"(%union._LARGE_INTEGER*) #0

declare dllimport x86_stdcallcc i32 @"\01_ReadFile@20"(i8*, i8*, i32, i32*, %struct._OVERLAPPED*) #0

declare dllimport x86_stdcallcc i32 @"\01_SetEndOfFile@4"(i8*) #0

declare dllimport x86_stdcallcc i32 @"\01_SetFilePointer@16"(i8*, i32, i32*, i32) #0

declare dllimport x86_stdcallcc i32 @"\01_SystemTimeToFileTime@8"(%struct._SYSTEMTIME*, %struct._FILETIME*) #0

declare dllimport x86_stdcallcc i32 @"\01_UnlockFile@20"(i8*, i32, i32, i32, i32) #0

declare dllimport x86_stdcallcc i32 @"\01_UnlockFileEx@20"(i8*, i32, i32, i32, %struct._OVERLAPPED*) #0

declare dllimport x86_stdcallcc i32 @"\01_UnmapViewOfFile@4"(i8*) #0

declare dllimport x86_stdcallcc i32 @"\01_WideCharToMultiByte@32"(i32, i32, i16*, i32, i8*, i32, i8*, i32*) #0

declare dllimport x86_stdcallcc i32 @"\01_WriteFile@20"(i8*, i8*, i32, i32*, %struct._OVERLAPPED*) #0

declare dllimport x86_stdcallcc i32 @"\01_WaitForSingleObject@8"(i8*, i32) #0

declare dllimport x86_stdcallcc i32 @"\01_WaitForSingleObjectEx@12"(i8*, i32, i32) #0

declare dllimport x86_stdcallcc void @"\01_OutputDebugStringA@4"(i8*) #0

declare dllimport x86_stdcallcc void @"\01_OutputDebugStringW@4"(i16*) #0

declare dllimport x86_stdcallcc i8* @"\01_GetProcessHeap@0"() #0

declare dllimport x86_stdcallcc i32 @"\01_FlushViewOfFile@8"(i8*, i32) #0

declare dllimport x86_stdcallcc void @"\01_EnterCriticalSection@4"(%struct._RTL_CRITICAL_SECTION*) #0

declare dllimport x86_stdcallcc void @"\01_LeaveCriticalSection@4"(%struct._RTL_CRITICAL_SECTION*) #0

declare dllimport x86_stdcallcc void @"\01_InitializeCriticalSection@4"(%struct._RTL_CRITICAL_SECTION*) #0

declare dllimport x86_stdcallcc i32 @"\01_ReleaseSemaphore@12"(i8*, i32, i32*) #0

declare dllimport x86_stdcallcc i8* @"\01_CreateSemaphoreA@16"(%struct._SECURITY_ATTRIBUTES*, i32, i32, i8*) #0

declare dllimport x86_stdcallcc i32 @"\01_SetThreadAffinityMask@8"(i8*, i32) #0

declare dllimport x86_stdcallcc i32 @"\01_GetCurrentThreadId@0"() #0

declare dllimport x86_stdcallcc i8* @"\01_CreateThread@24"(%struct._SECURITY_ATTRIBUTES*, i32, i32 (i8*)*, i8*, i32, i32*) #0

declare dllimport x86_stdcallcc i32 @"\01_SuspendThread@4"(i8*) #0

declare dllimport x86_stdcallcc i32 @"\01_ResumeThread@4"(i8*) #0

declare dllimport x86_stdcallcc i32 @"\01_SetThreadPriority@8"(i8*, i32) #0

declare dllimport x86_stdcallcc i32 @"\01_SetThreadPriorityBoost@8"(i8*, i32) #0

declare dllimport x86_stdcallcc i8* @"\01_CreateEventA@16"(%struct._SECURITY_ATTRIBUTES*, i32, i32, i8*) #0

declare dllimport x86_stdcallcc i32 @"\01_ResetEvent@4"(i8*) #0

declare dllimport x86_stdcallcc i32 @"\01_timeSetEvent@20"(i32, i32, void (i32, i32, i32, i32, i32)*, i32, i32) #0

declare dllimport x86_stdcallcc i32 @"\01_timeKillEvent@4"(i32) #0

declare dllimport x86_stdcallcc i32 @"\01_timeEndPeriod@4"(i32) #0

declare dllimport x86_stdcallcc i32 @"\01_timeGetDevCaps@8"(%struct.timecaps_tag*, i32) #0

declare dllimport x86_stdcallcc i32 @"\01_timeBeginPeriod@4"(i32) #0

declare dllimport x86_stdcallcc i32 @"\01_SetPriorityClass@8"(i8*, i32) #0

declare i32 @vprintf(i8*, i8*) #0

declare void @"\01?MyCreateThread@@YAXP6GIPAX@Z0H@Z"(i32 (i8*)*, i8*, i32) #0

declare void @"\01?MyPauseThread@@YAXXZ"() #0

declare void @"\01?MyWaitForObject@@YAXXZ"() #0

declare void @"\01?setNodeLength@@YAXH@Z"(i32) #0

declare void @"\01?$$Push@@YAXPAD0H@Z"(i8*, i8*, i32) #0

declare void @"\01?GetMsvVar@@YAXPAHP6AXXZP6A_NXZ@Z"(i32*, void ()*, i1 ()*) #0

declare void @"\01?addStatement@@YAXH@Z"(i32) #0

declare void @"\01?GetModelVar@@YAXPAY0BIGKA@DPAHP6AXXZ@Z"([100000 x i8]*, i32*, void ()*) #0

declare void @"\01?whileBegin@@YAXII@Z"(i32, i32) #0

declare void @"\01?whileEnd@@YAXII@Z"(i32, i32) #0

declare void @"\01?setModelOption@@YAX_N@Z"(i1 zeroext) #0

declare double @strtod(i8*, i8**) #0

; Function Attrs: noreturn
declare void @exit(i32) #2

define void @"\01?ex1@@YAXXZ"() #0 {
  call void @longjmp(i32* getelementptr inbounds ([16 x i32], [16 x i32]* @"\01?j_buf@@3PAHA", i32 0, i32 0), i32 0) #11
  unreachable
                                                  ; No predecessors!
  ret void
}

; Function Attrs: nounwind
define i8* @memcpy(i8* %dest, i8* %src, i32 %count) #3 {
  %1 = alloca i32, align 4
  %2 = alloca i8*, align 4
  %3 = alloca i8*, align 4
  %tmp_dest = alloca i8*, align 4
  %tmp_src = alloca i8*, align 4
  store i32 %count, i32* %1, align 4
  store i8* %src, i8** %2, align 4
  store i8* %dest, i8** %3, align 4
  %4 = load i8*, i8** %3, align 4
  store i8* %4, i8** %tmp_dest, align 4
  %5 = load i8*, i8** %2, align 4
  store i8* %5, i8** %tmp_src, align 4
  br label %6

; <label>:6                                       ; preds = %10, %0
  %7 = load i32, i32* %1, align 4
  %8 = add i32 %7, -1
  store i32 %8, i32* %1, align 4
  %9 = icmp ne i32 %7, 0
  br i1 %9, label %10, label %16

; <label>:10                                      ; preds = %6
  %11 = load i8*, i8** %tmp_src, align 4
  %12 = getelementptr inbounds i8, i8* %11, i32 1
  store i8* %12, i8** %tmp_src, align 4
  %13 = load i8, i8* %11, align 1
  %14 = load i8*, i8** %tmp_dest, align 4
  %15 = getelementptr inbounds i8, i8* %14, i32 1
  store i8* %15, i8** %tmp_dest, align 4
  store i8 %13, i8* %14, align 1
  br label %6

; <label>:16                                      ; preds = %6
  %17 = load i8*, i8** %3, align 4
  ret i8* %17
}

; Function Attrs: nounwind
define i8* @"\01?memmove@@YAPAXPAXPBXH@Z"(i8* %dest, i8* %src, i32 %num) #3 {
  %1 = alloca i8*, align 4
  %2 = alloca i32, align 4
  %3 = alloca i8*, align 4
  %4 = alloca i8*, align 4
  %tmp_dest = alloca i8*, align 4
  %tmp_src = alloca i8*, align 4
  store i32 %num, i32* %2, align 4
  store i8* %src, i8** %3, align 4
  store i8* %dest, i8** %4, align 4
  %5 = load i8*, i8** %4, align 4
  %6 = icmp eq i8* %5, null
  br i1 %6, label %10, label %7

; <label>:7                                       ; preds = %0
  %8 = load i8*, i8** %3, align 4
  %9 = icmp eq i8* %8, null
  br i1 %9, label %10, label %11

; <label>:10                                      ; preds = %7, %0
  store i8* null, i8** %1
  br label %59

; <label>:11                                      ; preds = %7
  %12 = load i8*, i8** %4, align 4
  store i8* %12, i8** %tmp_dest, align 4
  %13 = load i8*, i8** %3, align 4
  store i8* %13, i8** %tmp_src, align 4
  %14 = load i8*, i8** %tmp_dest, align 4
  %15 = load i32, i32* %2, align 4
  %16 = getelementptr inbounds i8, i8* %14, i32 %15
  %17 = load i8*, i8** %tmp_src, align 4
  %18 = icmp ult i8* %16, %17
  br i1 %18, label %25, label %19

; <label>:19                                      ; preds = %11
  %20 = load i8*, i8** %tmp_src, align 4
  %21 = load i32, i32* %2, align 4
  %22 = getelementptr inbounds i8, i8* %20, i32 %21
  %23 = load i8*, i8** %tmp_dest, align 4
  %24 = icmp ult i8* %22, %23
  br i1 %24, label %25, label %37

; <label>:25                                      ; preds = %19, %11
  br label %26

; <label>:26                                      ; preds = %30, %25
  %27 = load i32, i32* %2, align 4
  %28 = add nsw i32 %27, -1
  store i32 %28, i32* %2, align 4
  %29 = icmp ne i32 %27, 0
  br i1 %29, label %30, label %36

; <label>:30                                      ; preds = %26
  %31 = load i8*, i8** %tmp_src, align 4
  %32 = getelementptr inbounds i8, i8* %31, i32 1
  store i8* %32, i8** %tmp_src, align 4
  %33 = load i8, i8* %31, align 1
  %34 = load i8*, i8** %tmp_dest, align 4
  %35 = getelementptr inbounds i8, i8* %34, i32 1
  store i8* %35, i8** %tmp_dest, align 4
  store i8 %33, i8* %34, align 1
  br label %26

; <label>:36                                      ; preds = %26
  br label %57

; <label>:37                                      ; preds = %19
  %38 = load i8*, i8** %tmp_dest, align 4
  %39 = load i32, i32* %2, align 4
  %40 = getelementptr inbounds i8, i8* %38, i32 %39
  %41 = getelementptr inbounds i8, i8* %40, i32 -1
  store i8* %41, i8** %tmp_dest, align 4
  %42 = load i8*, i8** %tmp_src, align 4
  %43 = load i32, i32* %2, align 4
  %44 = getelementptr inbounds i8, i8* %42, i32 %43
  %45 = getelementptr inbounds i8, i8* %44, i32 -1
  store i8* %45, i8** %tmp_src, align 4
  br label %46

; <label>:46                                      ; preds = %50, %37
  %47 = load i32, i32* %2, align 4
  %48 = add nsw i32 %47, -1
  store i32 %48, i32* %2, align 4
  %49 = icmp ne i32 %47, 0
  br i1 %49, label %50, label %56

; <label>:50                                      ; preds = %46
  %51 = load i8*, i8** %tmp_src, align 4
  %52 = getelementptr inbounds i8, i8* %51, i32 -1
  store i8* %52, i8** %tmp_src, align 4
  %53 = load i8, i8* %51, align 1
  %54 = load i8*, i8** %tmp_dest, align 4
  %55 = getelementptr inbounds i8, i8* %54, i32 -1
  store i8* %55, i8** %tmp_dest, align 4
  store i8 %53, i8* %54, align 1
  br label %46

; <label>:56                                      ; preds = %46
  br label %57

; <label>:57                                      ; preds = %56, %36
  %58 = load i8*, i8** %4, align 4
  store i8* %58, i8** %1
  br label %59

; <label>:59                                      ; preds = %57, %10
  %60 = load i8*, i8** %1
  ret i8* %60
}

; Function Attrs: nounwind
define i8* @memset(i8* %s, i32 %c, i32 %n) #3 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i8*, align 4
  %p = alloca i8*, align 4
  store i32 %n, i32* %1, align 4
  store i32 %c, i32* %2, align 4
  store i8* %s, i8** %3, align 4
  %4 = load i8*, i8** %3, align 4
  store i8* %4, i8** %p, align 4
  br label %5

; <label>:5                                       ; preds = %8, %0
  %6 = load i32, i32* %1, align 4
  %7 = icmp ugt i32 %6, 0
  br i1 %7, label %8, label %15

; <label>:8                                       ; preds = %5
  %9 = load i32, i32* %2, align 4
  %10 = trunc i32 %9 to i8
  %11 = load i8*, i8** %p, align 4
  %12 = getelementptr inbounds i8, i8* %11, i32 1
  store i8* %12, i8** %p, align 4
  store i8 %10, i8* %11, align 1
  %13 = load i32, i32* %1, align 4
  %14 = add i32 %13, -1
  store i32 %14, i32* %1, align 4
  br label %5

; <label>:15                                      ; preds = %5
  %16 = load i8*, i8** %3, align 4
  ret i8* %16
}

; Function Attrs: nounwind
define void @"\01?$$addChar@@YAXD@Z"(i8 signext %ch) #3 {
  %1 = alloca i8, align 1
  store i8 %ch, i8* %1, align 1
  %2 = load i8, i8* %1, align 1
  %3 = load i32, i32* @"\01?$$tempLen@@3HA", align 4
  %4 = getelementptr inbounds [100000 x i8], [100000 x i8]* @"\01?$$varsInfo@@3PADA", i32 0, i32 %3
  store i8 %2, i8* %4, align 1
  %5 = load i32, i32* @"\01?$$tempLen@@3HA", align 4
  %6 = add nsw i32 %5, 1
  store i32 %6, i32* @"\01?$$tempLen@@3HA", align 4
  %7 = load i32, i32* @"\01?$$tempLen@@3HA", align 4
  %8 = getelementptr inbounds [100000 x i8], [100000 x i8]* @"\01?$$varsInfo@@3PADA", i32 0, i32 %7
  store i8 0, i8* %8, align 1
  ret void
}

define void @"\01?$$addInt@@YAXH@Z"(i32 %num) #0 {
  %1 = alloca i32, align 4
  %temp = alloca [10 x i8], align 1
  store i32 %num, i32* %1, align 4
  %2 = bitcast [10 x i8]* %temp to i8*
  call void @llvm.memset.p0i8.i32(i8* %2, i8 0, i32 10, i32 1, i1 false)
  %3 = load i32, i32* %1, align 4
  %4 = getelementptr inbounds [10 x i8], [10 x i8]* %temp, i32 0, i32 0
  %5 = call i32 (i8*, i8*, ...)* @sprintf(i8* %4, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @"\01??_C@_02DPKJAMEF@?$CFd?$AA@", i32 0, i32 0), i32 %3)
  %6 = getelementptr inbounds [10 x i8], [10 x i8]* %temp, i32 0, i32 0
  %7 = call i8* @strcat(i8* getelementptr inbounds ([100000 x i8], [100000 x i8]* @"\01?$$varsInfo@@3PADA", i32 0, i32 0), i8* %6)
  %8 = getelementptr inbounds [10 x i8], [10 x i8]* %temp, i32 0, i32 0
  %9 = call i32 @strlen(i8* %8)
  %10 = load i32, i32* @"\01?$$tempLen@@3HA", align 4
  %11 = add i32 %10, %9
  store i32 %11, i32* @"\01?$$tempLen@@3HA", align 4
  %12 = load i32, i32* @"\01?$$tempLen@@3HA", align 4
  %13 = getelementptr inbounds [100000 x i8], [100000 x i8]* @"\01?$$varsInfo@@3PADA", i32 0, i32 %12
  store i8 0, i8* %13, align 1
  ret void
}

define void @"\01?$$addFloat@@YAXM@Z"(float %num) #0 {
  %1 = alloca float, align 4
  %temp = alloca [30 x i8], align 1
  store float %num, float* %1, align 4
  %2 = bitcast [30 x i8]* %temp to i8*
  call void @llvm.memset.p0i8.i32(i8* %2, i8 0, i32 30, i32 1, i1 false)
  %3 = load float, float* %1, align 4
  %4 = fpext float %3 to double
  %5 = getelementptr inbounds [30 x i8], [30 x i8]* %temp, i32 0, i32 0
  %6 = call i32 (i8*, i8*, ...)* @sprintf(i8* %5, i8* getelementptr inbounds ([3 x i8], [3 x i8]* @"\01??_C@_02NJPGOMH@?$CFf?$AA@", i32 0, i32 0), double %4)
  %7 = getelementptr inbounds [30 x i8], [30 x i8]* %temp, i32 0, i32 0
  %8 = call i8* @strcat(i8* getelementptr inbounds ([100000 x i8], [100000 x i8]* @"\01?$$varsInfo@@3PADA", i32 0, i32 0), i8* %7)
  %9 = getelementptr inbounds [30 x i8], [30 x i8]* %temp, i32 0, i32 0
  %10 = call i32 @strlen(i8* %9)
  %11 = load i32, i32* @"\01?$$tempLen@@3HA", align 4
  %12 = add i32 %11, %10
  store i32 %12, i32* @"\01?$$tempLen@@3HA", align 4
  %13 = load i32, i32* @"\01?$$tempLen@@3HA", align 4
  %14 = getelementptr inbounds [100000 x i8], [100000 x i8]* @"\01?$$varsInfo@@3PADA", i32 0, i32 %13
  store i8 0, i8* %14, align 1
  ret void
}

define void @"\01?$$addString@@YAXPAD@Z"(i8* %str) #0 {
  %1 = alloca i8*, align 4
  store i8* %str, i8** %1, align 4
  %2 = load i8*, i8** %1, align 4
  %3 = call i8* @strcat(i8* getelementptr inbounds ([100000 x i8], [100000 x i8]* @"\01?$$varsInfo@@3PADA", i32 0, i32 0), i8* %2)
  %4 = load i8*, i8** %1, align 4
  %5 = call i32 @strlen(i8* %4)
  %6 = load i32, i32* @"\01?$$tempLen@@3HA", align 4
  %7 = add i32 %6, %5
  store i32 %7, i32* @"\01?$$tempLen@@3HA", align 4
  %8 = load i32, i32* @"\01?$$tempLen@@3HA", align 4
  %9 = getelementptr inbounds [100000 x i8], [100000 x i8]* @"\01?$$varsInfo@@3PADA", i32 0, i32 %8
  store i8 0, i8* %9, align 1
  ret void
}

define linkonce_odr void @"\01??__E_Generic_object@?$_Error_objects@H@std@@2V_Generic_error_category@2@A@YAXXZ"() comdat {
  %1 = call x86_thiscallcc %"class.std::_Generic_error_category"* @"\01??0_Generic_error_category@std@@QAE@XZ"(%"class.std::_Generic_error_category"* @"\01?_Generic_object@?$_Error_objects@H@std@@2V_Generic_error_category@2@A")
  %2 = call i32 @atexit(void ()* @"\01??__F_Generic_object@?$_Error_objects@H@std@@2V_Generic_error_category@2@A@YAXXZ") #5
  ret void
}

define linkonce_odr x86_thiscallcc %"class.std::_Generic_error_category"* @"\01??0_Generic_error_category@std@@QAE@XZ"(%"class.std::_Generic_error_category"* returned %this) unnamed_addr #0 comdat align 2 {
  %1 = alloca %"class.std::_Generic_error_category"*, align 4
  store %"class.std::_Generic_error_category"* %this, %"class.std::_Generic_error_category"** %1, align 4
  %2 = load %"class.std::_Generic_error_category"*, %"class.std::_Generic_error_category"** %1
  %3 = bitcast %"class.std::_Generic_error_category"* %2 to %"class.std::error_category"*
  %4 = call x86_thiscallcc %"class.std::error_category"* @"\01??0error_category@std@@QAE@XZ"(%"class.std::error_category"* %3)
  %5 = bitcast %"class.std::_Generic_error_category"* %2 to i32 (...)***
  store i32 (...)** bitcast (i8** @"\01??_7_Generic_error_category@std@@6B@" to i32 (...)**), i32 (...)*** %5
  ret %"class.std::_Generic_error_category"* %2
}

; Function Attrs: inlinehint nounwind
define linkonce_odr x86_thiscallcc void @"\01??1_Generic_error_category@std@@UAE@XZ"(%"class.std::_Generic_error_category"* %this) unnamed_addr #8 comdat align 2 {
  %1 = alloca %"class.std::_Generic_error_category"*, align 4
  store %"class.std::_Generic_error_category"* %this, %"class.std::_Generic_error_category"** %1, align 4
  %2 = load %"class.std::_Generic_error_category"*, %"class.std::_Generic_error_category"** %1
  %3 = bitcast %"class.std::_Generic_error_category"* %2 to %"class.std::error_category"*
  call x86_thiscallcc void @"\01??1error_category@std@@UAE@XZ"(%"class.std::error_category"* %3) #5
  ret void
}

define internal void @"\01??__F_Generic_object@?$_Error_objects@H@std@@2V_Generic_error_category@2@A@YAXXZ"() {
  call x86_thiscallcc void @"\01??1_Generic_error_category@std@@UAE@XZ"(%"class.std::_Generic_error_category"* @"\01?_Generic_object@?$_Error_objects@H@std@@2V_Generic_error_category@2@A")
  ret void
}

; Function Attrs: nounwind
declare i32 @atexit(void ()*) #5

define linkonce_odr void @"\01??__E_Iostream_object@?$_Error_objects@H@std@@2V_Iostream_error_category@2@A@YAXXZ"() comdat {
  %1 = call x86_thiscallcc %"class.std::_Iostream_error_category"* @"\01??0_Iostream_error_category@std@@QAE@XZ"(%"class.std::_Iostream_error_category"* @"\01?_Iostream_object@?$_Error_objects@H@std@@2V_Iostream_error_category@2@A")
  %2 = call i32 @atexit(void ()* @"\01??__F_Iostream_object@?$_Error_objects@H@std@@2V_Iostream_error_category@2@A@YAXXZ") #5
  ret void
}

define linkonce_odr x86_thiscallcc %"class.std::_Iostream_error_category"* @"\01??0_Iostream_error_category@std@@QAE@XZ"(%"class.std::_Iostream_error_category"* returned %this) unnamed_addr #0 comdat align 2 {
  %1 = alloca %"class.std::_Iostream_error_category"*, align 4
  store %"class.std::_Iostream_error_category"* %this, %"class.std::_Iostream_error_category"** %1, align 4
  %2 = load %"class.std::_Iostream_error_category"*, %"class.std::_Iostream_error_category"** %1
  %3 = bitcast %"class.std::_Iostream_error_category"* %2 to %"class.std::_Generic_error_category"*
  %4 = call x86_thiscallcc %"class.std::_Generic_error_category"* @"\01??0_Generic_error_category@std@@QAE@XZ"(%"class.std::_Generic_error_category"* %3)
  %5 = bitcast %"class.std::_Iostream_error_category"* %2 to i32 (...)***
  store i32 (...)** bitcast (i8** @"\01??_7_Iostream_error_category@std@@6B@" to i32 (...)**), i32 (...)*** %5
  ret %"class.std::_Iostream_error_category"* %2
}

; Function Attrs: inlinehint nounwind
define linkonce_odr x86_thiscallcc void @"\01??1_Iostream_error_category@std@@UAE@XZ"(%"class.std::_Iostream_error_category"* %this) unnamed_addr #8 comdat align 2 {
  %1 = alloca %"class.std::_Iostream_error_category"*, align 4
  store %"class.std::_Iostream_error_category"* %this, %"class.std::_Iostream_error_category"** %1, align 4
  %2 = load %"class.std::_Iostream_error_category"*, %"class.std::_Iostream_error_category"** %1
  %3 = bitcast %"class.std::_Iostream_error_category"* %2 to %"class.std::_Generic_error_category"*
  call x86_thiscallcc void @"\01??1_Generic_error_category@std@@UAE@XZ"(%"class.std::_Generic_error_category"* %3) #5
  ret void
}

define internal void @"\01??__F_Iostream_object@?$_Error_objects@H@std@@2V_Iostream_error_category@2@A@YAXXZ"() {
  call x86_thiscallcc void @"\01??1_Iostream_error_category@std@@UAE@XZ"(%"class.std::_Iostream_error_category"* @"\01?_Iostream_object@?$_Error_objects@H@std@@2V_Iostream_error_category@2@A")
  ret void
}

define linkonce_odr void @"\01??__E_System_object@?$_Error_objects@H@std@@2V_System_error_category@2@A@YAXXZ"() comdat {
  %1 = call x86_thiscallcc %"class.std::_System_error_category"* @"\01??0_System_error_category@std@@QAE@XZ"(%"class.std::_System_error_category"* @"\01?_System_object@?$_Error_objects@H@std@@2V_System_error_category@2@A")
  %2 = call i32 @atexit(void ()* @"\01??__F_System_object@?$_Error_objects@H@std@@2V_System_error_category@2@A@YAXXZ") #5
  ret void
}

define linkonce_odr x86_thiscallcc %"class.std::_System_error_category"* @"\01??0_System_error_category@std@@QAE@XZ"(%"class.std::_System_error_category"* returned %this) unnamed_addr #0 comdat align 2 {
  %1 = alloca %"class.std::_System_error_category"*, align 4
  store %"class.std::_System_error_category"* %this, %"class.std::_System_error_category"** %1, align 4
  %2 = load %"class.std::_System_error_category"*, %"class.std::_System_error_category"** %1
  %3 = bitcast %"class.std::_System_error_category"* %2 to %"class.std::_Generic_error_category"*
  %4 = call x86_thiscallcc %"class.std::_Generic_error_category"* @"\01??0_Generic_error_category@std@@QAE@XZ"(%"class.std::_Generic_error_category"* %3)
  %5 = bitcast %"class.std::_System_error_category"* %2 to i32 (...)***
  store i32 (...)** bitcast (i8** @"\01??_7_System_error_category@std@@6B@" to i32 (...)**), i32 (...)*** %5
  ret %"class.std::_System_error_category"* %2
}

; Function Attrs: inlinehint nounwind
define linkonce_odr x86_thiscallcc void @"\01??1_System_error_category@std@@UAE@XZ"(%"class.std::_System_error_category"* %this) unnamed_addr #8 comdat align 2 {
  %1 = alloca %"class.std::_System_error_category"*, align 4
  store %"class.std::_System_error_category"* %this, %"class.std::_System_error_category"** %1, align 4
  %2 = load %"class.std::_System_error_category"*, %"class.std::_System_error_category"** %1
  %3 = bitcast %"class.std::_System_error_category"* %2 to %"class.std::_Generic_error_category"*
  call x86_thiscallcc void @"\01??1_Generic_error_category@std@@UAE@XZ"(%"class.std::_Generic_error_category"* %3) #5
  ret void
}

define internal void @"\01??__F_System_object@?$_Error_objects@H@std@@2V_System_error_category@2@A@YAXXZ"() {
  call x86_thiscallcc void @"\01??1_System_error_category@std@@UAE@XZ"(%"class.std::_System_error_category"* @"\01?_System_object@?$_Error_objects@H@std@@2V_System_error_category@2@A")
  ret void
}

declare i64 @_time64(i64*) #0

declare %struct.tm* @_localtime64(i64*) #0

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc %"class.std::error_category"* @"\01??0error_category@std@@QAE@XZ"(%"class.std::error_category"* returned %this) unnamed_addr #3 comdat align 2 {
  %1 = alloca %"class.std::error_category"*, align 4
  store %"class.std::error_category"* %this, %"class.std::error_category"** %1, align 4
  %2 = load %"class.std::error_category"*, %"class.std::error_category"** %1
  %3 = bitcast %"class.std::error_category"* %2 to i32 (...)***
  store i32 (...)** bitcast (i8** @"\01??_7error_category@std@@6B@" to i32 (...)**), i32 (...)*** %3
  ret %"class.std::error_category"* %2
}

; Function Attrs: inlinehint nounwind
define linkonce_odr x86_thiscallcc i8* @"\01??_G_Generic_error_category@std@@UAEPAXI@Z"(%"class.std::_Generic_error_category"* %this, i32 %should_call_delete) unnamed_addr #8 comdat align 2 {
  %1 = alloca i8*, align 4
  %2 = alloca i32, align 4
  %3 = alloca %"class.std::_Generic_error_category"*, align 4
  store i32 %should_call_delete, i32* %2, align 4
  store %"class.std::_Generic_error_category"* %this, %"class.std::_Generic_error_category"** %3, align 4
  %4 = load %"class.std::_Generic_error_category"*, %"class.std::_Generic_error_category"** %3
  %5 = bitcast %"class.std::_Generic_error_category"* %4 to i8*
  store i8* %5, i8** %1
  %6 = load i32, i32* %2
  call x86_thiscallcc void @"\01??1_Generic_error_category@std@@UAE@XZ"(%"class.std::_Generic_error_category"* %4) #5
  %7 = icmp eq i32 %6, 0
  br i1 %7, label %10, label %8

; <label>:8                                       ; preds = %0
  %9 = bitcast %"class.std::_Generic_error_category"* %4 to i8*
  call void @"\01??3@YAXPAX@Z"(i8* %9) #13
  br label %10

; <label>:10                                      ; preds = %8, %0
  %11 = load i8*, i8** %1
  ret i8* %11
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc i8* @"\01?name@_Generic_error_category@std@@UBEPBDXZ"(%"class.std::_Generic_error_category"* %this) unnamed_addr #3 comdat align 2 {
  %1 = alloca %"class.std::_Generic_error_category"*, align 4
  store %"class.std::_Generic_error_category"* %this, %"class.std::_Generic_error_category"** %1, align 4
  %2 = load %"class.std::_Generic_error_category"*, %"class.std::_Generic_error_category"** %1
  ret i8* getelementptr inbounds ([8 x i8], [8 x i8]* @"\01??_C@_07DCLBNMLN@generic?$AA@", i32 0, i32 0)
}

define linkonce_odr x86_thiscallcc void @"\01?message@_Generic_error_category@std@@UBE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@2@H@Z"(%"class.std::_Generic_error_category"* %this, %"class.std::basic_string"* noalias sret %agg.result, i32 %_Errcode) unnamed_addr #0 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca %"class.std::_Generic_error_category"*, align 4
  %_Name = alloca i8*, align 4
  store i32 %_Errcode, i32* %1, align 4
  store %"class.std::_Generic_error_category"* %this, %"class.std::_Generic_error_category"** %2, align 4
  %3 = load %"class.std::_Generic_error_category"*, %"class.std::_Generic_error_category"** %2
  %4 = load i32, i32* %1, align 4
  %5 = call i8* @"\01?_Syserror_map@std@@YAPBDH@Z"(i32 %4)
  store i8* %5, i8** %_Name, align 4
  %6 = load i8*, i8** %_Name, align 4
  %7 = icmp ne i8* %6, null
  br i1 %7, label %8, label %10

; <label>:8                                       ; preds = %0
  %9 = load i8*, i8** %_Name, align 4
  br label %11

; <label>:10                                      ; preds = %0
  br label %11

; <label>:11                                      ; preds = %10, %8
  %12 = phi i8* [ %9, %8 ], [ getelementptr inbounds ([14 x i8], [14 x i8]* @"\01??_C@_0O@BFJCFAAK@unknown?5error?$AA@", i32 0, i32 0), %10 ]
  %13 = call x86_thiscallcc %"class.std::basic_string"* @"\01??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z"(%"class.std::basic_string"* %agg.result, i8* %12)
  ret void
}

; Function Attrs: inlinehint nounwind
define linkonce_odr x86_thiscallcc void @"\01?default_error_condition@error_category@std@@UBE?AVerror_condition@2@H@Z"(%"class.std::error_category"* %this, %"class.std::error_condition"* noalias sret %agg.result, i32 %_Errval) unnamed_addr #8 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca %"class.std::error_category"*, align 4
  store i32 %_Errval, i32* %1, align 4
  store %"class.std::error_category"* %this, %"class.std::error_category"** %2, align 4
  %3 = load %"class.std::error_category"*, %"class.std::error_category"** %2
  %4 = load i32, i32* %1, align 4
  %5 = call x86_thiscallcc %"class.std::error_condition"* @"\01??0error_condition@std@@QAE@HABVerror_category@1@@Z"(%"class.std::error_condition"* %agg.result, i32 %4, %"class.std::error_category"* dereferenceable(4) %3) #5
  ret void
}

; Function Attrs: inlinehint nounwind
define linkonce_odr x86_thiscallcc zeroext i1 @"\01?equivalent@error_category@std@@UBE_NABVerror_code@2@H@Z"(%"class.std::error_category"* %this, %"class.std::error_code"* dereferenceable(8) %_Code, i32 %_Errval) unnamed_addr #8 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca %"class.std::error_code"*, align 4
  %3 = alloca %"class.std::error_category"*, align 4
  %4 = alloca i8*
  %5 = alloca i32
  store i32 %_Errval, i32* %1, align 4
  store %"class.std::error_code"* %_Code, %"class.std::error_code"** %2, align 4
  store %"class.std::error_category"* %this, %"class.std::error_category"** %3, align 4
  %6 = load %"class.std::error_category"*, %"class.std::error_category"** %3
  %7 = load %"class.std::error_code"*, %"class.std::error_code"** %2, align 4
  %8 = invoke x86_thiscallcc dereferenceable(4) %"class.std::error_category"* @"\01?category@error_code@std@@QBEABVerror_category@2@XZ"(%"class.std::error_code"* %7)
          to label %9 unwind label %19

; <label>:9                                       ; preds = %0
  %10 = call x86_thiscallcc zeroext i1 @"\01??8error_category@std@@QBE_NABV01@@Z"(%"class.std::error_category"* %6, %"class.std::error_category"* dereferenceable(4) %8) #5
  br i1 %10, label %11, label %17

; <label>:11                                      ; preds = %9
  %12 = load %"class.std::error_code"*, %"class.std::error_code"** %2, align 4
  %13 = invoke x86_thiscallcc i32 @"\01?value@error_code@std@@QBEHXZ"(%"class.std::error_code"* %12)
          to label %14 unwind label %19

; <label>:14                                      ; preds = %11
  %15 = load i32, i32* %1, align 4
  %16 = icmp eq i32 %13, %15
  br label %17

; <label>:17                                      ; preds = %14, %9
  %18 = phi i1 [ false, %9 ], [ %16, %14 ]
  ret i1 %18

; <label>:19                                      ; preds = %11, %0
  %20 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__CxxFrameHandler3 to i8*)
          filter [0 x i8*] zeroinitializer
  %21 = extractvalue { i8*, i32 } %20, 0
  store i8* %21, i8** %4
  %22 = extractvalue { i8*, i32 } %20, 1
  store i32 %22, i32* %5
  br label %23

; <label>:23                                      ; preds = %19
  %24 = load i8*, i8** %4
  call void @__cxa_call_unexpected(i8* %24) #11
  unreachable
}

; Function Attrs: inlinehint nounwind
define linkonce_odr x86_thiscallcc zeroext i1 @"\01?equivalent@error_category@std@@UBE_NHABVerror_condition@2@@Z"(%"class.std::error_category"* %this, i32 %_Errval, %"class.std::error_condition"* dereferenceable(8) %_Cond) unnamed_addr #8 comdat align 2 {
  %1 = alloca %"class.std::error_condition"*, align 4
  %2 = alloca i32, align 4
  %3 = alloca %"class.std::error_category"*, align 4
  %4 = alloca %"class.std::error_condition", align 4
  store %"class.std::error_condition"* %_Cond, %"class.std::error_condition"** %1, align 4
  store i32 %_Errval, i32* %2, align 4
  store %"class.std::error_category"* %this, %"class.std::error_category"** %3, align 4
  %5 = load %"class.std::error_category"*, %"class.std::error_category"** %3
  %6 = bitcast %"class.std::error_category"* %5 to void (%"class.std::error_category"*, %"class.std::error_condition"*, i32)***
  %7 = load void (%"class.std::error_category"*, %"class.std::error_condition"*, i32)**, void (%"class.std::error_category"*, %"class.std::error_condition"*, i32)*** %6
  %8 = getelementptr inbounds void (%"class.std::error_category"*, %"class.std::error_condition"*, i32)*, void (%"class.std::error_category"*, %"class.std::error_condition"*, i32)** %7, i64 3
  %9 = load void (%"class.std::error_category"*, %"class.std::error_condition"*, i32)*, void (%"class.std::error_category"*, %"class.std::error_condition"*, i32)** %8
  %10 = load i32, i32* %2, align 4
  call x86_thiscallcc void %9(%"class.std::error_category"* %5, %"class.std::error_condition"* sret %4, i32 %10) #5
  %11 = load %"class.std::error_condition"*, %"class.std::error_condition"** %1, align 4
  %12 = call x86_thiscallcc zeroext i1 @"\01??8error_condition@std@@QBE_NABV01@@Z"(%"class.std::error_condition"* %4, %"class.std::error_condition"* dereferenceable(8) %11) #5
  ret i1 %12
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc i8* @"\01??_Gerror_category@std@@UAEPAXI@Z"(%"class.std::error_category"* %this, i32 %should_call_delete) unnamed_addr #3 comdat align 2 {
  %1 = alloca i8*, align 4
  %2 = alloca i32, align 4
  %3 = alloca %"class.std::error_category"*, align 4
  store i32 %should_call_delete, i32* %2, align 4
  store %"class.std::error_category"* %this, %"class.std::error_category"** %3, align 4
  %4 = load %"class.std::error_category"*, %"class.std::error_category"** %3
  %5 = bitcast %"class.std::error_category"* %4 to i8*
  store i8* %5, i8** %1
  %6 = load i32, i32* %2
  call x86_thiscallcc void @"\01??1error_category@std@@UAE@XZ"(%"class.std::error_category"* %4) #5
  %7 = icmp eq i32 %6, 0
  br i1 %7, label %10, label %8

; <label>:8                                       ; preds = %0
  %9 = bitcast %"class.std::error_category"* %4 to i8*
  call void @"\01??3@YAXPAX@Z"(i8* %9) #13
  br label %10

; <label>:10                                      ; preds = %8, %0
  %11 = load i8*, i8** %1
  ret i8* %11
}

declare void @_purecall()

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc void @"\01??1error_category@std@@UAE@XZ"(%"class.std::error_category"* %this) unnamed_addr #3 comdat align 2 {
  %1 = alloca %"class.std::error_category"*, align 4
  store %"class.std::error_category"* %this, %"class.std::error_category"** %1, align 4
  %2 = load %"class.std::error_category"*, %"class.std::error_category"** %1
  ret void
}

; Function Attrs: nobuiltin nounwind
declare void @"\01??3@YAXPAX@Z"(i8*) #9

declare i8* @"\01?_Syserror_map@std@@YAPBDH@Z"(i32) #0

define linkonce_odr x86_thiscallcc %"class.std::basic_string"* @"\01??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z"(%"class.std::basic_string"* returned %this, i8* %_Ptr) unnamed_addr #0 comdat align 2 {
  %1 = alloca i8*, align 4
  %2 = alloca %"class.std::basic_string"*, align 4
  %3 = alloca %"class.std::allocator", align 1
  store i8* %_Ptr, i8** %1, align 4
  store %"class.std::basic_string"* %this, %"class.std::basic_string"** %2, align 4
  %4 = load %"class.std::basic_string"*, %"class.std::basic_string"** %2
  %5 = bitcast %"class.std::basic_string"* %4 to %"class.std::_String_alloc"*
  %6 = call x86_thiscallcc %"class.std::allocator"* @"\01??0?$allocator@D@std@@QAE@XZ"(%"class.std::allocator"* %3) #5
  %7 = call x86_thiscallcc %"class.std::_String_alloc"* @"\01??0?$_String_alloc@$0A@U?$_String_base_types@DV?$allocator@D@std@@@std@@@std@@QAE@ABV?$allocator@D@1@@Z"(%"class.std::_String_alloc"* %5, %"class.std::allocator"* dereferenceable(1) %3)
  call x86_thiscallcc void @"\01?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEX_NI@Z"(%"class.std::basic_string"* %4, i1 zeroext false, i32 0)
  %8 = load i8*, i8** %1, align 4
  %9 = call x86_thiscallcc dereferenceable(24) %"class.std::basic_string"* @"\01?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBD@Z"(%"class.std::basic_string"* %4, i8* %8)
  ret %"class.std::basic_string"* %4
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc %"class.std::allocator"* @"\01??0?$allocator@D@std@@QAE@XZ"(%"class.std::allocator"* returned %this) unnamed_addr #3 comdat align 2 {
  %1 = alloca %"class.std::allocator"*, align 4
  store %"class.std::allocator"* %this, %"class.std::allocator"** %1, align 4
  %2 = load %"class.std::allocator"*, %"class.std::allocator"** %1
  %3 = bitcast %"class.std::allocator"* %2 to %"struct.std::_Allocator_base"*
  ret %"class.std::allocator"* %2
}

define linkonce_odr x86_thiscallcc %"class.std::_String_alloc"* @"\01??0?$_String_alloc@$0A@U?$_String_base_types@DV?$allocator@D@std@@@std@@@std@@QAE@ABV?$allocator@D@1@@Z"(%"class.std::_String_alloc"* returned %this, %"class.std::allocator"* dereferenceable(1)) unnamed_addr #0 comdat align 2 {
  %2 = alloca %"class.std::allocator"*, align 4
  %3 = alloca %"class.std::_String_alloc"*, align 4
  store %"class.std::allocator"* %0, %"class.std::allocator"** %2, align 4
  store %"class.std::_String_alloc"* %this, %"class.std::_String_alloc"** %3, align 4
  %4 = load %"class.std::_String_alloc"*, %"class.std::_String_alloc"** %3
  %5 = bitcast %"class.std::_String_alloc"* %4 to %"class.std::_String_val"*
  %6 = call x86_thiscallcc %"class.std::_String_val"* @"\01??0?$_String_val@U?$_Simple_types@D@std@@@std@@QAE@XZ"(%"class.std::_String_val"* %5)
  ret %"class.std::_String_alloc"* %4
}

define linkonce_odr x86_thiscallcc void @"\01?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEX_NI@Z"(%"class.std::basic_string"* %this, i1 zeroext %_Built, i32 %_Newsize) #0 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca i8, align 1
  %3 = alloca %"class.std::basic_string"*, align 4
  %_Ptr = alloca i8*, align 4
  %4 = alloca %"struct.std::_Wrap_alloc", align 1
  %5 = alloca %"struct.std::_Wrap_alloc", align 1
  store i32 %_Newsize, i32* %1, align 4
  %6 = zext i1 %_Built to i8
  store i8 %6, i8* %2, align 1
  store %"class.std::basic_string"* %this, %"class.std::basic_string"** %3, align 4
  %7 = load %"class.std::basic_string"*, %"class.std::basic_string"** %3
  %8 = load i8, i8* %2, align 1
  %9 = trunc i8 %8 to i1
  br i1 %9, label %11, label %10

; <label>:10                                      ; preds = %0
  br label %44

; <label>:11                                      ; preds = %0
  %12 = bitcast %"class.std::basic_string"* %7 to %"class.std::_String_val"*
  %13 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %12, i32 0, i32 2
  %14 = load i32, i32* %13, align 4
  %15 = icmp ule i32 16, %14
  br i1 %15, label %16, label %43

; <label>:16                                      ; preds = %11
  %17 = bitcast %"class.std::basic_string"* %7 to %"class.std::_String_val"*
  %18 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %17, i32 0, i32 0
  %19 = bitcast %"union.std::_String_val<std::_Simple_types<char> >::_Bxty"* %18 to i8**
  %20 = load i8*, i8** %19, align 4
  store i8* %20, i8** %_Ptr, align 4
  %21 = bitcast %"class.std::basic_string"* %7 to %"class.std::_String_alloc"*
  call x86_thiscallcc void @"\01?_Getal@?$_String_alloc@$0A@U?$_String_base_types@DV?$allocator@D@std@@@std@@@std@@QBE?AU?$_Wrap_alloc@V?$allocator@D@std@@@2@XZ"(%"class.std::_String_alloc"* %21, %"struct.std::_Wrap_alloc"* sret %4)
  %22 = bitcast %"class.std::basic_string"* %7 to %"class.std::_String_val"*
  %23 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %22, i32 0, i32 0
  %24 = bitcast %"union.std::_String_val<std::_Simple_types<char> >::_Bxty"* %23 to i8**
  call x86_thiscallcc void @"\01??$destroy@PAD@?$_Wrap_alloc@V?$allocator@D@std@@@std@@QAEXPAPAD@Z"(%"struct.std::_Wrap_alloc"* %4, i8** %24)
  %25 = load i32, i32* %1, align 4
  %26 = icmp ult i32 0, %25
  br i1 %26, label %27, label %36

; <label>:27                                      ; preds = %16
  %28 = load i32, i32* %1, align 4
  %29 = load i8*, i8** %_Ptr, align 4
  %30 = call i8* @"\01??$addressof@D@std@@YAPADAAD@Z"(i8* dereferenceable(1) %29) #5
  %31 = bitcast %"class.std::basic_string"* %7 to %"class.std::_String_val"*
  %32 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %31, i32 0, i32 0
  %33 = bitcast %"union.std::_String_val<std::_Simple_types<char> >::_Bxty"* %32 to [16 x i8]*
  %34 = getelementptr inbounds [16 x i8], [16 x i8]* %33, i32 0, i32 0
  %35 = call i8* @"\01?copy@?$char_traits@D@std@@SAPADPADPBDI@Z"(i8* %34, i8* %30, i32 %28)
  br label %36

; <label>:36                                      ; preds = %27, %16
  %37 = bitcast %"class.std::basic_string"* %7 to %"class.std::_String_alloc"*
  call x86_thiscallcc void @"\01?_Getal@?$_String_alloc@$0A@U?$_String_base_types@DV?$allocator@D@std@@@std@@@std@@QBE?AU?$_Wrap_alloc@V?$allocator@D@std@@@2@XZ"(%"class.std::_String_alloc"* %37, %"struct.std::_Wrap_alloc"* sret %5)
  %38 = bitcast %"class.std::basic_string"* %7 to %"class.std::_String_val"*
  %39 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %38, i32 0, i32 2
  %40 = load i32, i32* %39, align 4
  %41 = add i32 %40, 1
  %42 = load i8*, i8** %_Ptr, align 4
  call x86_thiscallcc void @"\01?deallocate@?$_Wrap_alloc@V?$allocator@D@std@@@std@@QAEXPADI@Z"(%"struct.std::_Wrap_alloc"* %5, i8* %42, i32 %41)
  br label %43

; <label>:43                                      ; preds = %36, %11
  br label %44

; <label>:44                                      ; preds = %43, %10
  %45 = bitcast %"class.std::basic_string"* %7 to %"class.std::_String_val"*
  %46 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %45, i32 0, i32 2
  store i32 15, i32* %46, align 4
  %47 = load i32, i32* %1, align 4
  call x86_thiscallcc void @"\01?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEXI@Z"(%"class.std::basic_string"* %7, i32 %47)
  ret void
}

define linkonce_odr x86_thiscallcc dereferenceable(24) %"class.std::basic_string"* @"\01?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBD@Z"(%"class.std::basic_string"* %this, i8* %_Ptr) #0 comdat align 2 {
  %1 = alloca i8*, align 4
  %2 = alloca %"class.std::basic_string"*, align 4
  store i8* %_Ptr, i8** %1, align 4
  store %"class.std::basic_string"* %this, %"class.std::basic_string"** %2, align 4
  %3 = load %"class.std::basic_string"*, %"class.std::basic_string"** %2
  %4 = load i8*, i8** %1, align 4
  %5 = call i32 @"\01?length@?$char_traits@D@std@@SAIPBD@Z"(i8* %4)
  %6 = load i8*, i8** %1, align 4
  %7 = call x86_thiscallcc dereferenceable(24) %"class.std::basic_string"* @"\01?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBDI@Z"(%"class.std::basic_string"* %3, i8* %6, i32 %5)
  ret %"class.std::basic_string"* %7
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc %"class.std::_String_val"* @"\01??0?$_String_val@U?$_Simple_types@D@std@@@std@@QAE@XZ"(%"class.std::_String_val"* returned %this) unnamed_addr #3 comdat align 2 {
  %1 = alloca %"class.std::_String_val"*, align 4
  store %"class.std::_String_val"* %this, %"class.std::_String_val"** %1, align 4
  %2 = load %"class.std::_String_val"*, %"class.std::_String_val"** %1
  %3 = bitcast %"class.std::_String_val"* %2 to %"struct.std::_Container_base0"*
  %4 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %2, i32 0, i32 0
  %5 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %2, i32 0, i32 1
  store i32 0, i32* %5, align 4
  %6 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %2, i32 0, i32 2
  store i32 0, i32* %6, align 4
  ret %"class.std::_String_val"* %2
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc void @"\01?_Getal@?$_String_alloc@$0A@U?$_String_base_types@DV?$allocator@D@std@@@std@@@std@@QBE?AU?$_Wrap_alloc@V?$allocator@D@std@@@2@XZ"(%"class.std::_String_alloc"* %this, %"struct.std::_Wrap_alloc"* noalias sret %agg.result) #3 comdat align 2 {
  %1 = alloca %"class.std::_String_alloc"*, align 4
  store %"class.std::_String_alloc"* %this, %"class.std::_String_alloc"** %1, align 4
  %2 = load %"class.std::_String_alloc"*, %"class.std::_String_alloc"** %1
  %3 = call x86_thiscallcc %"struct.std::_Wrap_alloc"* @"\01??0?$_Wrap_alloc@V?$allocator@D@std@@@std@@QAE@XZ"(%"struct.std::_Wrap_alloc"* %agg.result) #5
  ret void
}

define linkonce_odr x86_thiscallcc void @"\01??$destroy@PAD@?$_Wrap_alloc@V?$allocator@D@std@@@std@@QAEXPAPAD@Z"(%"struct.std::_Wrap_alloc"* %this, i8** %_Ptr) #0 comdat align 2 {
  %1 = alloca i8**, align 4
  %2 = alloca %"struct.std::_Wrap_alloc"*, align 4
  store i8** %_Ptr, i8*** %1, align 4
  store %"struct.std::_Wrap_alloc"* %this, %"struct.std::_Wrap_alloc"** %2, align 4
  %3 = load %"struct.std::_Wrap_alloc"*, %"struct.std::_Wrap_alloc"** %2
  %4 = load i8**, i8*** %1, align 4
  %5 = bitcast %"struct.std::_Wrap_alloc"* %3 to %"class.std::allocator"*
  call void @"\01??$destroy@PAD@?$allocator_traits@V?$allocator@D@std@@@std@@SAXAAV?$allocator@D@1@PAPAD@Z"(%"class.std::allocator"* dereferenceable(1) %5, i8** %4)
  ret void
}

; Function Attrs: nounwind
define linkonce_odr i8* @"\01?copy@?$char_traits@D@std@@SAPADPADPBDI@Z"(i8* %_First1, i8* %_First2, i32 %_Count) #3 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca i8*, align 4
  %3 = alloca i8*, align 4
  store i32 %_Count, i32* %1, align 4
  store i8* %_First2, i8** %2, align 4
  store i8* %_First1, i8** %3, align 4
  %4 = load i32, i32* %1, align 4
  %5 = icmp eq i32 %4, 0
  br i1 %5, label %6, label %8

; <label>:6                                       ; preds = %0
  %7 = load i8*, i8** %3, align 4
  br label %12

; <label>:8                                       ; preds = %0
  %9 = load i8*, i8** %3, align 4
  %10 = load i8*, i8** %2, align 4
  %11 = load i32, i32* %1, align 4
  call void @llvm.memcpy.p0i8.p0i8.i32(i8* %9, i8* %10, i32 %11, i32 1, i1 false)
  br label %12

; <label>:12                                      ; preds = %8, %6
  %13 = phi i8* [ %7, %6 ], [ %9, %8 ]
  ret i8* %13
}

; Function Attrs: inlinehint nounwind
define linkonce_odr i8* @"\01??$addressof@D@std@@YAPADAAD@Z"(i8* dereferenceable(1) %_Val) #8 comdat {
  %1 = alloca i8*, align 4
  store i8* %_Val, i8** %1, align 4
  %2 = load i8*, i8** %1, align 4
  ret i8* %2
}

define linkonce_odr x86_thiscallcc void @"\01?deallocate@?$_Wrap_alloc@V?$allocator@D@std@@@std@@QAEXPADI@Z"(%"struct.std::_Wrap_alloc"* %this, i8* %_Ptr, i32 %_Count) #0 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca i8*, align 4
  %3 = alloca %"struct.std::_Wrap_alloc"*, align 4
  store i32 %_Count, i32* %1, align 4
  store i8* %_Ptr, i8** %2, align 4
  store %"struct.std::_Wrap_alloc"* %this, %"struct.std::_Wrap_alloc"** %3, align 4
  %4 = load %"struct.std::_Wrap_alloc"*, %"struct.std::_Wrap_alloc"** %3
  %5 = bitcast %"struct.std::_Wrap_alloc"* %4 to %"class.std::allocator"*
  %6 = load i32, i32* %1, align 4
  %7 = load i8*, i8** %2, align 4
  call x86_thiscallcc void @"\01?deallocate@?$allocator@D@std@@QAEXPADI@Z"(%"class.std::allocator"* %5, i8* %7, i32 %6)
  ret void
}

define linkonce_odr x86_thiscallcc void @"\01?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEXI@Z"(%"class.std::basic_string"* %this, i32 %_Newsize) #0 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca %"class.std::basic_string"*, align 4
  %3 = alloca i8, align 1
  store i32 %_Newsize, i32* %1, align 4
  store %"class.std::basic_string"* %this, %"class.std::basic_string"** %2, align 4
  %4 = load %"class.std::basic_string"*, %"class.std::basic_string"** %2
  store i8 0, i8* %3
  %5 = load i32, i32* %1, align 4
  %6 = bitcast %"class.std::basic_string"* %4 to %"class.std::_String_val"*
  %7 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %6, i32 0, i32 1
  store i32 %5, i32* %7, align 4
  %8 = bitcast %"class.std::basic_string"* %4 to %"class.std::_String_val"*
  %9 = call x86_thiscallcc i8* @"\01?_Myptr@?$_String_val@U?$_Simple_types@D@std@@@std@@QAEPADXZ"(%"class.std::_String_val"* %8)
  %10 = getelementptr inbounds i8, i8* %9, i32 %5
  call void @"\01?assign@?$char_traits@D@std@@SAXAADABD@Z"(i8* dereferenceable(1) %10, i8* dereferenceable(1) %3) #5
  ret void
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc %"struct.std::_Wrap_alloc"* @"\01??0?$_Wrap_alloc@V?$allocator@D@std@@@std@@QAE@XZ"(%"struct.std::_Wrap_alloc"* returned %this) unnamed_addr #3 comdat align 2 {
  %1 = alloca %"struct.std::_Wrap_alloc"*, align 4
  store %"struct.std::_Wrap_alloc"* %this, %"struct.std::_Wrap_alloc"** %1, align 4
  %2 = load %"struct.std::_Wrap_alloc"*, %"struct.std::_Wrap_alloc"** %1
  %3 = bitcast %"struct.std::_Wrap_alloc"* %2 to %"class.std::allocator"*
  %4 = call x86_thiscallcc %"class.std::allocator"* @"\01??0?$allocator@D@std@@QAE@XZ"(%"class.std::allocator"* %3) #5
  ret %"struct.std::_Wrap_alloc"* %2
}

define linkonce_odr void @"\01??$destroy@PAD@?$allocator_traits@V?$allocator@D@std@@@std@@SAXAAV?$allocator@D@1@PAPAD@Z"(%"class.std::allocator"* dereferenceable(1) %_Al, i8** %_Ptr) #0 comdat align 2 {
  %1 = alloca i8**, align 4
  %2 = alloca %"class.std::allocator"*, align 4
  store i8** %_Ptr, i8*** %1, align 4
  store %"class.std::allocator"* %_Al, %"class.std::allocator"** %2, align 4
  %3 = load %"class.std::allocator"*, %"class.std::allocator"** %2, align 4
  %4 = load i8**, i8*** %1, align 4
  call x86_thiscallcc void @"\01??$destroy@PAD@?$allocator@D@std@@QAEXPAPAD@Z"(%"class.std::allocator"* %3, i8** %4)
  ret void
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc void @"\01??$destroy@PAD@?$allocator@D@std@@QAEXPAPAD@Z"(%"class.std::allocator"* %this, i8** %_Ptr) #3 comdat align 2 {
  %1 = alloca i8**, align 4
  %2 = alloca %"class.std::allocator"*, align 4
  store i8** %_Ptr, i8*** %1, align 4
  store %"class.std::allocator"* %this, %"class.std::allocator"** %2, align 4
  %3 = load %"class.std::allocator"*, %"class.std::allocator"** %2
  %4 = load i8**, i8*** %1, align 4
  ret void
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc void @"\01?deallocate@?$allocator@D@std@@QAEXPADI@Z"(%"class.std::allocator"* %this, i8* %_Ptr, i32) #3 comdat align 2 {
  %2 = alloca i32, align 4
  %3 = alloca i8*, align 4
  %4 = alloca %"class.std::allocator"*, align 4
  store i32 %0, i32* %2, align 4
  store i8* %_Ptr, i8** %3, align 4
  store %"class.std::allocator"* %this, %"class.std::allocator"** %4, align 4
  %5 = load %"class.std::allocator"*, %"class.std::allocator"** %4
  %6 = load i8*, i8** %3, align 4
  call void @"\01??3@YAXPAX@Z"(i8* %6) #5
  ret void
}

; Function Attrs: nounwind
define linkonce_odr void @"\01?assign@?$char_traits@D@std@@SAXAADABD@Z"(i8* dereferenceable(1) %_Left, i8* dereferenceable(1) %_Right) #3 comdat align 2 {
  %1 = alloca i8*, align 4
  %2 = alloca i8*, align 4
  store i8* %_Right, i8** %1, align 4
  store i8* %_Left, i8** %2, align 4
  %3 = load i8*, i8** %1, align 4
  %4 = load i8, i8* %3, align 1
  %5 = load i8*, i8** %2, align 4
  store i8 %4, i8* %5, align 1
  ret void
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc i8* @"\01?_Myptr@?$_String_val@U?$_Simple_types@D@std@@@std@@QAEPADXZ"(%"class.std::_String_val"* %this) #3 comdat align 2 {
  %1 = alloca %"class.std::_String_val"*, align 4
  store %"class.std::_String_val"* %this, %"class.std::_String_val"** %1, align 4
  %2 = load %"class.std::_String_val"*, %"class.std::_String_val"** %1
  %3 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %2, i32 0, i32 2
  %4 = load i32, i32* %3, align 4
  %5 = icmp ule i32 16, %4
  br i1 %5, label %6, label %11

; <label>:6                                       ; preds = %0
  %7 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %2, i32 0, i32 0
  %8 = bitcast %"union.std::_String_val<std::_Simple_types<char> >::_Bxty"* %7 to i8**
  %9 = load i8*, i8** %8, align 4
  %10 = call i8* @"\01??$addressof@D@std@@YAPADAAD@Z"(i8* dereferenceable(1) %9) #5
  br label %15

; <label>:11                                      ; preds = %0
  %12 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %2, i32 0, i32 0
  %13 = bitcast %"union.std::_String_val<std::_Simple_types<char> >::_Bxty"* %12 to [16 x i8]*
  %14 = getelementptr inbounds [16 x i8], [16 x i8]* %13, i32 0, i32 0
  br label %15

; <label>:15                                      ; preds = %11, %6
  %16 = phi i8* [ %10, %6 ], [ %14, %11 ]
  ret i8* %16
}

define linkonce_odr x86_thiscallcc dereferenceable(24) %"class.std::basic_string"* @"\01?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBDI@Z"(%"class.std::basic_string"* %this, i8* %_Ptr, i32 %_Count) #0 comdat align 2 {
  %1 = alloca %"class.std::basic_string"*, align 4
  %2 = alloca i32, align 4
  %3 = alloca i8*, align 4
  %4 = alloca %"class.std::basic_string"*, align 4
  store i32 %_Count, i32* %2, align 4
  store i8* %_Ptr, i8** %3, align 4
  store %"class.std::basic_string"* %this, %"class.std::basic_string"** %4, align 4
  %5 = load %"class.std::basic_string"*, %"class.std::basic_string"** %4
  %6 = load i8*, i8** %3, align 4
  %7 = call x86_thiscallcc zeroext i1 @"\01?_Inside@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE_NPBD@Z"(%"class.std::basic_string"* %5, i8* %6)
  br i1 %7, label %8, label %17

; <label>:8                                       ; preds = %0
  %9 = load i32, i32* %2, align 4
  %10 = load i8*, i8** %3, align 4
  %11 = bitcast %"class.std::basic_string"* %5 to %"class.std::_String_val"*
  %12 = call x86_thiscallcc i8* @"\01?_Myptr@?$_String_val@U?$_Simple_types@D@std@@@std@@QAEPADXZ"(%"class.std::_String_val"* %11)
  %13 = ptrtoint i8* %10 to i32
  %14 = ptrtoint i8* %12 to i32
  %15 = sub i32 %13, %14
  %16 = call x86_thiscallcc dereferenceable(24) %"class.std::basic_string"* @"\01?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@ABV12@II@Z"(%"class.std::basic_string"* %5, %"class.std::basic_string"* dereferenceable(24) %5, i32 %15, i32 %9)
  store %"class.std::basic_string"* %16, %"class.std::basic_string"** %1
  br label %28

; <label>:17                                      ; preds = %0
  %18 = load i32, i32* %2, align 4
  %19 = call x86_thiscallcc zeroext i1 @"\01?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE_NI_N@Z"(%"class.std::basic_string"* %5, i32 %18, i1 zeroext false)
  br i1 %19, label %20, label %27

; <label>:20                                      ; preds = %17
  %21 = load i32, i32* %2, align 4
  %22 = load i8*, i8** %3, align 4
  %23 = bitcast %"class.std::basic_string"* %5 to %"class.std::_String_val"*
  %24 = call x86_thiscallcc i8* @"\01?_Myptr@?$_String_val@U?$_Simple_types@D@std@@@std@@QAEPADXZ"(%"class.std::_String_val"* %23)
  %25 = call i8* @"\01?copy@?$char_traits@D@std@@SAPADPADPBDI@Z"(i8* %24, i8* %22, i32 %21)
  %26 = load i32, i32* %2, align 4
  call x86_thiscallcc void @"\01?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEXI@Z"(%"class.std::basic_string"* %5, i32 %26)
  br label %27

; <label>:27                                      ; preds = %20, %17
  store %"class.std::basic_string"* %5, %"class.std::basic_string"** %1
  br label %28

; <label>:28                                      ; preds = %27, %8
  %29 = load %"class.std::basic_string"*, %"class.std::basic_string"** %1
  ret %"class.std::basic_string"* %29
}

define linkonce_odr i32 @"\01?length@?$char_traits@D@std@@SAIPBD@Z"(i8* %_First) #0 comdat align 2 {
  %1 = alloca i8*, align 4
  store i8* %_First, i8** %1, align 4
  %2 = load i8*, i8** %1, align 4
  %3 = load i8, i8* %2, align 1
  %4 = sext i8 %3 to i32
  %5 = icmp eq i32 %4, 0
  br i1 %5, label %6, label %7

; <label>:6                                       ; preds = %0
  br label %10

; <label>:7                                       ; preds = %0
  %8 = load i8*, i8** %1, align 4
  %9 = call i32 @strlen(i8* %8)
  br label %10

; <label>:10                                      ; preds = %7, %6
  %11 = phi i32 [ 0, %6 ], [ %9, %7 ]
  ret i32 %11
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc zeroext i1 @"\01?_Inside@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE_NPBD@Z"(%"class.std::basic_string"* %this, i8* %_Ptr) #3 comdat align 2 {
  %1 = alloca i1, align 1
  %2 = alloca i8*, align 4
  %3 = alloca %"class.std::basic_string"*, align 4
  store i8* %_Ptr, i8** %2, align 4
  store %"class.std::basic_string"* %this, %"class.std::basic_string"** %3, align 4
  %4 = load %"class.std::basic_string"*, %"class.std::basic_string"** %3
  %5 = load i8*, i8** %2, align 4
  %6 = icmp eq i8* %5, null
  br i1 %6, label %21, label %7

; <label>:7                                       ; preds = %0
  %8 = load i8*, i8** %2, align 4
  %9 = bitcast %"class.std::basic_string"* %4 to %"class.std::_String_val"*
  %10 = call x86_thiscallcc i8* @"\01?_Myptr@?$_String_val@U?$_Simple_types@D@std@@@std@@QAEPADXZ"(%"class.std::_String_val"* %9)
  %11 = icmp ult i8* %8, %10
  br i1 %11, label %21, label %12

; <label>:12                                      ; preds = %7
  %13 = bitcast %"class.std::basic_string"* %4 to %"class.std::_String_val"*
  %14 = call x86_thiscallcc i8* @"\01?_Myptr@?$_String_val@U?$_Simple_types@D@std@@@std@@QAEPADXZ"(%"class.std::_String_val"* %13)
  %15 = bitcast %"class.std::basic_string"* %4 to %"class.std::_String_val"*
  %16 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %15, i32 0, i32 1
  %17 = load i32, i32* %16, align 4
  %18 = getelementptr inbounds i8, i8* %14, i32 %17
  %19 = load i8*, i8** %2, align 4
  %20 = icmp ule i8* %18, %19
  br i1 %20, label %21, label %22

; <label>:21                                      ; preds = %12, %7, %0
  store i1 false, i1* %1
  br label %23

; <label>:22                                      ; preds = %12
  store i1 true, i1* %1
  br label %23

; <label>:23                                      ; preds = %22, %21
  %24 = load i1, i1* %1
  ret i1 %24
}

define linkonce_odr x86_thiscallcc dereferenceable(24) %"class.std::basic_string"* @"\01?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@ABV12@II@Z"(%"class.std::basic_string"* %this, %"class.std::basic_string"* dereferenceable(24) %_Right, i32 %_Roff, i32 %_Count) #0 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca %"class.std::basic_string"*, align 4
  %4 = alloca %"class.std::basic_string"*, align 4
  %_Num = alloca i32, align 4
  store i32 %_Count, i32* %1, align 4
  store i32 %_Roff, i32* %2, align 4
  store %"class.std::basic_string"* %_Right, %"class.std::basic_string"** %3, align 4
  store %"class.std::basic_string"* %this, %"class.std::basic_string"** %4, align 4
  %5 = load %"class.std::basic_string"*, %"class.std::basic_string"** %4
  %6 = load %"class.std::basic_string"*, %"class.std::basic_string"** %3, align 4
  %7 = call x86_thiscallcc i32 @"\01?size@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ"(%"class.std::basic_string"* %6) #5
  %8 = load i32, i32* %2, align 4
  %9 = icmp ult i32 %7, %8
  br i1 %9, label %10, label %11

; <label>:10                                      ; preds = %0
  call x86_thiscallcc void @"\01?_Xran@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEXXZ"(%"class.std::basic_string"* %5) #11
  unreachable

; <label>:11                                      ; preds = %0
  %12 = load %"class.std::basic_string"*, %"class.std::basic_string"** %3, align 4
  %13 = call x86_thiscallcc i32 @"\01?size@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ"(%"class.std::basic_string"* %12) #5
  %14 = load i32, i32* %2, align 4
  %15 = sub i32 %13, %14
  store i32 %15, i32* %_Num, align 4
  %16 = load i32, i32* %1, align 4
  %17 = load i32, i32* %_Num, align 4
  %18 = icmp ult i32 %16, %17
  br i1 %18, label %19, label %21

; <label>:19                                      ; preds = %11
  %20 = load i32, i32* %1, align 4
  store i32 %20, i32* %_Num, align 4
  br label %21

; <label>:21                                      ; preds = %19, %11
  %22 = load %"class.std::basic_string"*, %"class.std::basic_string"** %3, align 4
  %23 = icmp eq %"class.std::basic_string"* %5, %22
  br i1 %23, label %24, label %31

; <label>:24                                      ; preds = %21
  %25 = load i32, i32* %2, align 4
  %26 = load i32, i32* %_Num, align 4
  %27 = add i32 %25, %26
  %28 = call x86_thiscallcc dereferenceable(24) %"class.std::basic_string"* @"\01?erase@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@I@Z"(%"class.std::basic_string"* %5, i32 %27)
  %29 = load i32, i32* %2, align 4
  %30 = call x86_thiscallcc dereferenceable(24) %"class.std::basic_string"* @"\01?erase@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@II@Z"(%"class.std::basic_string"* %5, i32 0, i32 %29)
  br label %46

; <label>:31                                      ; preds = %21
  %32 = load i32, i32* %_Num, align 4
  %33 = call x86_thiscallcc zeroext i1 @"\01?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE_NI_N@Z"(%"class.std::basic_string"* %5, i32 %32, i1 zeroext false)
  br i1 %33, label %34, label %45

; <label>:34                                      ; preds = %31
  %35 = load i32, i32* %_Num, align 4
  %36 = load %"class.std::basic_string"*, %"class.std::basic_string"** %3, align 4
  %37 = bitcast %"class.std::basic_string"* %36 to %"class.std::_String_val"*
  %38 = call x86_thiscallcc i8* @"\01?_Myptr@?$_String_val@U?$_Simple_types@D@std@@@std@@QBEPBDXZ"(%"class.std::_String_val"* %37)
  %39 = load i32, i32* %2, align 4
  %40 = getelementptr inbounds i8, i8* %38, i32 %39
  %41 = bitcast %"class.std::basic_string"* %5 to %"class.std::_String_val"*
  %42 = call x86_thiscallcc i8* @"\01?_Myptr@?$_String_val@U?$_Simple_types@D@std@@@std@@QAEPADXZ"(%"class.std::_String_val"* %41)
  %43 = call i8* @"\01?copy@?$char_traits@D@std@@SAPADPADPBDI@Z"(i8* %42, i8* %40, i32 %35)
  %44 = load i32, i32* %_Num, align 4
  call x86_thiscallcc void @"\01?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEXI@Z"(%"class.std::basic_string"* %5, i32 %44)
  br label %45

; <label>:45                                      ; preds = %34, %31
  br label %46

; <label>:46                                      ; preds = %45, %24
  ret %"class.std::basic_string"* %5
}

define linkonce_odr x86_thiscallcc zeroext i1 @"\01?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE_NI_N@Z"(%"class.std::basic_string"* %this, i32 %_Newsize, i1 zeroext %_Trim) #0 comdat align 2 {
  %1 = alloca i8, align 1
  %2 = alloca i32, align 4
  %3 = alloca %"class.std::basic_string"*, align 4
  %4 = zext i1 %_Trim to i8
  store i8 %4, i8* %1, align 1
  store i32 %_Newsize, i32* %2, align 4
  store %"class.std::basic_string"* %this, %"class.std::basic_string"** %3, align 4
  %5 = load %"class.std::basic_string"*, %"class.std::basic_string"** %3
  %6 = call x86_thiscallcc i32 @"\01?max_size@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ"(%"class.std::basic_string"* %5) #5
  %7 = load i32, i32* %2, align 4
  %8 = icmp ult i32 %6, %7
  br i1 %8, label %9, label %10

; <label>:9                                       ; preds = %0
  call x86_thiscallcc void @"\01?_Xlen@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEXXZ"(%"class.std::basic_string"* %5) #11
  unreachable

; <label>:10                                      ; preds = %0
  %11 = bitcast %"class.std::basic_string"* %5 to %"class.std::_String_val"*
  %12 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %11, i32 0, i32 2
  %13 = load i32, i32* %12, align 4
  %14 = load i32, i32* %2, align 4
  %15 = icmp ult i32 %13, %14
  br i1 %15, label %16, label %21

; <label>:16                                      ; preds = %10
  %17 = bitcast %"class.std::basic_string"* %5 to %"class.std::_String_val"*
  %18 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %17, i32 0, i32 1
  %19 = load i32, i32* %18, align 4
  %20 = load i32, i32* %2, align 4
  call x86_thiscallcc void @"\01?_Copy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEXII@Z"(%"class.std::basic_string"* %5, i32 %20, i32 %19)
  br label %47

; <label>:21                                      ; preds = %10
  %22 = load i8, i8* %1, align 1
  %23 = trunc i8 %22 to i1
  br i1 %23, label %24, label %41

; <label>:24                                      ; preds = %21
  %25 = load i32, i32* %2, align 4
  %26 = icmp ult i32 %25, 16
  br i1 %26, label %27, label %41

; <label>:27                                      ; preds = %24
  %28 = load i32, i32* %2, align 4
  %29 = bitcast %"class.std::basic_string"* %5 to %"class.std::_String_val"*
  %30 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %29, i32 0, i32 1
  %31 = load i32, i32* %30, align 4
  %32 = icmp ult i32 %28, %31
  br i1 %32, label %33, label %35

; <label>:33                                      ; preds = %27
  %34 = load i32, i32* %2, align 4
  br label %39

; <label>:35                                      ; preds = %27
  %36 = bitcast %"class.std::basic_string"* %5 to %"class.std::_String_val"*
  %37 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %36, i32 0, i32 1
  %38 = load i32, i32* %37, align 4
  br label %39

; <label>:39                                      ; preds = %35, %33
  %40 = phi i32 [ %34, %33 ], [ %38, %35 ]
  call x86_thiscallcc void @"\01?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEX_NI@Z"(%"class.std::basic_string"* %5, i1 zeroext true, i32 %40)
  br label %46

; <label>:41                                      ; preds = %24, %21
  %42 = load i32, i32* %2, align 4
  %43 = icmp eq i32 %42, 0
  br i1 %43, label %44, label %45

; <label>:44                                      ; preds = %41
  call x86_thiscallcc void @"\01?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEXI@Z"(%"class.std::basic_string"* %5, i32 0)
  br label %45

; <label>:45                                      ; preds = %44, %41
  br label %46

; <label>:46                                      ; preds = %45, %39
  br label %47

; <label>:47                                      ; preds = %46, %16
  %48 = load i32, i32* %2, align 4
  %49 = icmp ult i32 0, %48
  ret i1 %49
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc i32 @"\01?size@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ"(%"class.std::basic_string"* %this) #3 comdat align 2 {
  %1 = alloca %"class.std::basic_string"*, align 4
  store %"class.std::basic_string"* %this, %"class.std::basic_string"** %1, align 4
  %2 = load %"class.std::basic_string"*, %"class.std::basic_string"** %1
  %3 = bitcast %"class.std::basic_string"* %2 to %"class.std::_String_val"*
  %4 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %3, i32 0, i32 1
  %5 = load i32, i32* %4, align 4
  ret i32 %5
}

; Function Attrs: noreturn
define linkonce_odr x86_thiscallcc void @"\01?_Xran@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEXXZ"(%"class.std::basic_string"* %this) #2 comdat align 2 {
  %1 = alloca %"class.std::basic_string"*, align 4
  store %"class.std::basic_string"* %this, %"class.std::basic_string"** %1, align 4
  %2 = load %"class.std::basic_string"*, %"class.std::basic_string"** %1
  call void @"\01?_Xout_of_range@std@@YAXPBD@Z"(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @"\01??_C@_0BI@CFPLBAOH@invalid?5string?5position?$AA@", i32 0, i32 0)) #11
  unreachable
                                                  ; No predecessors!
  ret void
}

define linkonce_odr x86_thiscallcc dereferenceable(24) %"class.std::basic_string"* @"\01?erase@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@I@Z"(%"class.std::basic_string"* %this, i32 %_Off) #0 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca %"class.std::basic_string"*, align 4
  store i32 %_Off, i32* %1, align 4
  store %"class.std::basic_string"* %this, %"class.std::basic_string"** %2, align 4
  %3 = load %"class.std::basic_string"*, %"class.std::basic_string"** %2
  %4 = bitcast %"class.std::basic_string"* %3 to %"class.std::_String_val"*
  %5 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %4, i32 0, i32 1
  %6 = load i32, i32* %5, align 4
  %7 = load i32, i32* %1, align 4
  %8 = icmp ult i32 %6, %7
  br i1 %8, label %9, label %10

; <label>:9                                       ; preds = %0
  call x86_thiscallcc void @"\01?_Xran@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEXXZ"(%"class.std::basic_string"* %3) #11
  unreachable

; <label>:10                                      ; preds = %0
  %11 = load i32, i32* %1, align 4
  call x86_thiscallcc void @"\01?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEXI@Z"(%"class.std::basic_string"* %3, i32 %11)
  ret %"class.std::basic_string"* %3
}

define linkonce_odr x86_thiscallcc dereferenceable(24) %"class.std::basic_string"* @"\01?erase@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@II@Z"(%"class.std::basic_string"* %this, i32 %_Off, i32 %_Count) #0 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca %"class.std::basic_string"*, align 4
  %_Ptr = alloca i8*, align 4
  %_Newsize = alloca i32, align 4
  store i32 %_Count, i32* %1, align 4
  store i32 %_Off, i32* %2, align 4
  store %"class.std::basic_string"* %this, %"class.std::basic_string"** %3, align 4
  %4 = load %"class.std::basic_string"*, %"class.std::basic_string"** %3
  %5 = bitcast %"class.std::basic_string"* %4 to %"class.std::_String_val"*
  %6 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %5, i32 0, i32 1
  %7 = load i32, i32* %6, align 4
  %8 = load i32, i32* %2, align 4
  %9 = icmp ult i32 %7, %8
  br i1 %9, label %10, label %11

; <label>:10                                      ; preds = %0
  call x86_thiscallcc void @"\01?_Xran@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEXXZ"(%"class.std::basic_string"* %4) #11
  unreachable

; <label>:11                                      ; preds = %0
  %12 = bitcast %"class.std::basic_string"* %4 to %"class.std::_String_val"*
  %13 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %12, i32 0, i32 1
  %14 = load i32, i32* %13, align 4
  %15 = load i32, i32* %2, align 4
  %16 = sub i32 %14, %15
  %17 = load i32, i32* %1, align 4
  %18 = icmp ule i32 %16, %17
  br i1 %18, label %19, label %21

; <label>:19                                      ; preds = %11
  %20 = load i32, i32* %2, align 4
  call x86_thiscallcc void @"\01?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEXI@Z"(%"class.std::basic_string"* %4, i32 %20)
  br label %44

; <label>:21                                      ; preds = %11
  %22 = load i32, i32* %1, align 4
  %23 = icmp ult i32 0, %22
  br i1 %23, label %24, label %43

; <label>:24                                      ; preds = %21
  %25 = bitcast %"class.std::basic_string"* %4 to %"class.std::_String_val"*
  %26 = call x86_thiscallcc i8* @"\01?_Myptr@?$_String_val@U?$_Simple_types@D@std@@@std@@QAEPADXZ"(%"class.std::_String_val"* %25)
  %27 = load i32, i32* %2, align 4
  %28 = getelementptr inbounds i8, i8* %26, i32 %27
  store i8* %28, i8** %_Ptr, align 4
  %29 = bitcast %"class.std::basic_string"* %4 to %"class.std::_String_val"*
  %30 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %29, i32 0, i32 1
  %31 = load i32, i32* %30, align 4
  %32 = load i32, i32* %1, align 4
  %33 = sub i32 %31, %32
  store i32 %33, i32* %_Newsize, align 4
  %34 = load i32, i32* %_Newsize, align 4
  %35 = load i32, i32* %2, align 4
  %36 = sub i32 %34, %35
  %37 = load i8*, i8** %_Ptr, align 4
  %38 = load i32, i32* %1, align 4
  %39 = getelementptr inbounds i8, i8* %37, i32 %38
  %40 = load i8*, i8** %_Ptr, align 4
  %41 = call i8* @"\01?move@?$char_traits@D@std@@SAPADPADPBDI@Z"(i8* %40, i8* %39, i32 %36)
  %42 = load i32, i32* %_Newsize, align 4
  call x86_thiscallcc void @"\01?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEXI@Z"(%"class.std::basic_string"* %4, i32 %42)
  br label %43

; <label>:43                                      ; preds = %24, %21
  br label %44

; <label>:44                                      ; preds = %43, %19
  ret %"class.std::basic_string"* %4
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc i8* @"\01?_Myptr@?$_String_val@U?$_Simple_types@D@std@@@std@@QBEPBDXZ"(%"class.std::_String_val"* %this) #3 comdat align 2 {
  %1 = alloca %"class.std::_String_val"*, align 4
  store %"class.std::_String_val"* %this, %"class.std::_String_val"** %1, align 4
  %2 = load %"class.std::_String_val"*, %"class.std::_String_val"** %1
  %3 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %2, i32 0, i32 2
  %4 = load i32, i32* %3, align 4
  %5 = icmp ule i32 16, %4
  br i1 %5, label %6, label %11

; <label>:6                                       ; preds = %0
  %7 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %2, i32 0, i32 0
  %8 = bitcast %"union.std::_String_val<std::_Simple_types<char> >::_Bxty"* %7 to i8**
  %9 = load i8*, i8** %8, align 4
  %10 = call i8* @"\01??$addressof@D@std@@YAPADAAD@Z"(i8* dereferenceable(1) %9) #5
  br label %15

; <label>:11                                      ; preds = %0
  %12 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %2, i32 0, i32 0
  %13 = bitcast %"union.std::_String_val<std::_Simple_types<char> >::_Bxty"* %12 to [16 x i8]*
  %14 = getelementptr inbounds [16 x i8], [16 x i8]* %13, i32 0, i32 0
  br label %15

; <label>:15                                      ; preds = %11, %6
  %16 = phi i8* [ %10, %6 ], [ %14, %11 ]
  ret i8* %16
}

; Function Attrs: noreturn
declare void @"\01?_Xout_of_range@std@@YAXPBD@Z"(i8*) #2

; Function Attrs: nounwind
define linkonce_odr i8* @"\01?move@?$char_traits@D@std@@SAPADPADPBDI@Z"(i8* %_First1, i8* %_First2, i32 %_Count) #3 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca i8*, align 4
  %3 = alloca i8*, align 4
  store i32 %_Count, i32* %1, align 4
  store i8* %_First2, i8** %2, align 4
  store i8* %_First1, i8** %3, align 4
  %4 = load i32, i32* %1, align 4
  %5 = icmp eq i32 %4, 0
  br i1 %5, label %6, label %8

; <label>:6                                       ; preds = %0
  %7 = load i8*, i8** %3, align 4
  br label %12

; <label>:8                                       ; preds = %0
  %9 = load i8*, i8** %3, align 4
  %10 = load i8*, i8** %2, align 4
  %11 = load i32, i32* %1, align 4
  call void @llvm.memmove.p0i8.p0i8.i32(i8* %9, i8* %10, i32 %11, i32 1, i1 false)
  br label %12

; <label>:12                                      ; preds = %8, %6
  %13 = phi i8* [ %7, %6 ], [ %9, %8 ]
  ret i8* %13
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc i32 @"\01?max_size@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ"(%"class.std::basic_string"* %this) #3 comdat align 2 {
  %1 = alloca %"class.std::basic_string"*, align 4
  %_Num = alloca i32, align 4
  %2 = alloca %"struct.std::_Wrap_alloc", align 1
  %3 = alloca i8*
  %4 = alloca i32
  store %"class.std::basic_string"* %this, %"class.std::basic_string"** %1, align 4
  %5 = load %"class.std::basic_string"*, %"class.std::basic_string"** %1
  %6 = bitcast %"class.std::basic_string"* %5 to %"class.std::_String_alloc"*
  invoke x86_thiscallcc void @"\01?_Getal@?$_String_alloc@$0A@U?$_String_base_types@DV?$allocator@D@std@@@std@@@std@@QBE?AU?$_Wrap_alloc@V?$allocator@D@std@@@2@XZ"(%"class.std::_String_alloc"* %6, %"struct.std::_Wrap_alloc"* sret %2)
          to label %7 unwind label %17

; <label>:7                                       ; preds = %0
  %8 = call x86_thiscallcc i32 @"\01?max_size@?$_Wrap_alloc@V?$allocator@D@std@@@std@@QBEIXZ"(%"struct.std::_Wrap_alloc"* %2) #5
  store i32 %8, i32* %_Num, align 4
  %9 = load i32, i32* %_Num, align 4
  %10 = icmp ule i32 %9, 1
  br i1 %10, label %11, label %12

; <label>:11                                      ; preds = %7
  br label %15

; <label>:12                                      ; preds = %7
  %13 = load i32, i32* %_Num, align 4
  %14 = sub i32 %13, 1
  br label %15

; <label>:15                                      ; preds = %12, %11
  %16 = phi i32 [ 1, %11 ], [ %14, %12 ]
  ret i32 %16

; <label>:17                                      ; preds = %0
  %18 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__CxxFrameHandler3 to i8*)
          filter [0 x i8*] zeroinitializer
  %19 = extractvalue { i8*, i32 } %18, 0
  store i8* %19, i8** %3
  %20 = extractvalue { i8*, i32 } %18, 1
  store i32 %20, i32* %4
  br label %21

; <label>:21                                      ; preds = %17
  %22 = load i8*, i8** %3
  call void @__cxa_call_unexpected(i8* %22) #11
  unreachable
}

; Function Attrs: noreturn
define linkonce_odr x86_thiscallcc void @"\01?_Xlen@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEXXZ"(%"class.std::basic_string"* %this) #2 comdat align 2 {
  %1 = alloca %"class.std::basic_string"*, align 4
  store %"class.std::basic_string"* %this, %"class.std::basic_string"** %1, align 4
  %2 = load %"class.std::basic_string"*, %"class.std::basic_string"** %1
  call void @"\01?_Xlength_error@std@@YAXPBD@Z"(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @"\01??_C@_0BA@JFNIOLAK@string?5too?5long?$AA@", i32 0, i32 0)) #11
  unreachable
                                                  ; No predecessors!
  ret void
}

define linkonce_odr x86_thiscallcc void @"\01?_Copy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEXII@Z"(%"class.std::basic_string"* %this, i32 %_Newsize, i32 %_Oldlen) #0 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca %"class.std::basic_string"*, align 4
  %_Newres = alloca i32, align 4
  %_Ptr = alloca i8*, align 4
  %4 = alloca %"struct.std::_Wrap_alloc", align 1
  %5 = alloca i8*
  %6 = alloca i32
  %7 = alloca %"struct.std::_Wrap_alloc", align 1
  %8 = alloca %"struct.std::_Wrap_alloc", align 1
  store i32 %_Oldlen, i32* %1, align 4
  store i32 %_Newsize, i32* %2, align 4
  store %"class.std::basic_string"* %this, %"class.std::basic_string"** %3, align 4
  %9 = load %"class.std::basic_string"*, %"class.std::basic_string"** %3
  %10 = load i32, i32* %2, align 4
  %11 = or i32 %10, 15
  store i32 %11, i32* %_Newres, align 4
  %12 = call x86_thiscallcc i32 @"\01?max_size@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ"(%"class.std::basic_string"* %9) #5
  %13 = load i32, i32* %_Newres, align 4
  %14 = icmp ult i32 %12, %13
  br i1 %14, label %15, label %17

; <label>:15                                      ; preds = %0
  %16 = load i32, i32* %2, align 4
  store i32 %16, i32* %_Newres, align 4
  br label %50

; <label>:17                                      ; preds = %0
  %18 = bitcast %"class.std::basic_string"* %9 to %"class.std::_String_val"*
  %19 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %18, i32 0, i32 2
  %20 = load i32, i32* %19, align 4
  %21 = udiv i32 %20, 2
  %22 = load i32, i32* %_Newres, align 4
  %23 = udiv i32 %22, 3
  %24 = icmp ule i32 %21, %23
  br i1 %24, label %25, label %26

; <label>:25                                      ; preds = %17
  br label %49

; <label>:26                                      ; preds = %17
  %27 = bitcast %"class.std::basic_string"* %9 to %"class.std::_String_val"*
  %28 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %27, i32 0, i32 2
  %29 = load i32, i32* %28, align 4
  %30 = call x86_thiscallcc i32 @"\01?max_size@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ"(%"class.std::basic_string"* %9) #5
  %31 = bitcast %"class.std::basic_string"* %9 to %"class.std::_String_val"*
  %32 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %31, i32 0, i32 2
  %33 = load i32, i32* %32, align 4
  %34 = udiv i32 %33, 2
  %35 = sub i32 %30, %34
  %36 = icmp ule i32 %29, %35
  br i1 %36, label %37, label %46

; <label>:37                                      ; preds = %26
  %38 = bitcast %"class.std::basic_string"* %9 to %"class.std::_String_val"*
  %39 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %38, i32 0, i32 2
  %40 = load i32, i32* %39, align 4
  %41 = bitcast %"class.std::basic_string"* %9 to %"class.std::_String_val"*
  %42 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %41, i32 0, i32 2
  %43 = load i32, i32* %42, align 4
  %44 = udiv i32 %43, 2
  %45 = add i32 %40, %44
  store i32 %45, i32* %_Newres, align 4
  br label %48

; <label>:46                                      ; preds = %26
  %47 = call x86_thiscallcc i32 @"\01?max_size@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QBEIXZ"(%"class.std::basic_string"* %9) #5
  store i32 %47, i32* %_Newres, align 4
  br label %48

; <label>:48                                      ; preds = %46, %37
  br label %49

; <label>:49                                      ; preds = %48, %25
  br label %50

; <label>:50                                      ; preds = %49, %15
  %51 = bitcast %"class.std::basic_string"* %9 to %"class.std::_String_alloc"*
  invoke x86_thiscallcc void @"\01?_Getal@?$_String_alloc@$0A@U?$_String_base_types@DV?$allocator@D@std@@@std@@@std@@QBE?AU?$_Wrap_alloc@V?$allocator@D@std@@@2@XZ"(%"class.std::_String_alloc"* %51, %"struct.std::_Wrap_alloc"* sret %4)
          to label %52 unwind label %57

; <label>:52                                      ; preds = %50
  %53 = load i32, i32* %_Newres, align 4
  %54 = add i32 %53, 1
  %55 = invoke x86_thiscallcc i8* @"\01?allocate@?$_Wrap_alloc@V?$allocator@D@std@@@std@@QAEPADI@Z"(%"struct.std::_Wrap_alloc"* %4, i32 %54)
          to label %56 unwind label %57

; <label>:56                                      ; preds = %52
  store i8* %55, i8** %_Ptr, align 4
  br label %82

; <label>:57                                      ; preds = %52, %50
  %58 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__CxxFrameHandler3 to i8*)
          catch i8* null
  %59 = extractvalue { i8*, i32 } %58, 0
  store i8* %59, i8** %5
  %60 = extractvalue { i8*, i32 } %58, 1
  store i32 %60, i32* %6
  br label %61

; <label>:61                                      ; preds = %57
  %62 = load i8*, i8** %5
  call void @llvm.eh.begincatch(i8* %62, i8* null) #5
  %63 = load i32, i32* %2, align 4
  store i32 %63, i32* %_Newres, align 4
  %64 = bitcast %"class.std::basic_string"* %9 to %"class.std::_String_alloc"*
  invoke x86_thiscallcc void @"\01?_Getal@?$_String_alloc@$0A@U?$_String_base_types@DV?$allocator@D@std@@@std@@@std@@QBE?AU?$_Wrap_alloc@V?$allocator@D@std@@@2@XZ"(%"class.std::_String_alloc"* %64, %"struct.std::_Wrap_alloc"* sret %7)
          to label %65 unwind label %70

; <label>:65                                      ; preds = %61
  %66 = load i32, i32* %_Newres, align 4
  %67 = add i32 %66, 1
  %68 = invoke x86_thiscallcc i8* @"\01?allocate@?$_Wrap_alloc@V?$allocator@D@std@@@std@@QAEPADI@Z"(%"struct.std::_Wrap_alloc"* %7, i32 %67)
          to label %69 unwind label %70

; <label>:69                                      ; preds = %65
  store i8* %68, i8** %_Ptr, align 4
  br label %81

; <label>:70                                      ; preds = %65, %61
  %71 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__CxxFrameHandler3 to i8*)
          catch i8* null
  %72 = extractvalue { i8*, i32 } %71, 0
  store i8* %72, i8** %5
  %73 = extractvalue { i8*, i32 } %71, 1
  store i32 %73, i32* %6
  br label %74

; <label>:74                                      ; preds = %70
  %75 = load i8*, i8** %5
  call void @llvm.eh.begincatch(i8* %75, i8* null) #5
  invoke x86_thiscallcc void @"\01?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEX_NI@Z"(%"class.std::basic_string"* %9, i1 zeroext true, i32 0)
          to label %76 unwind label %77

; <label>:76                                      ; preds = %74
  invoke void @_CxxThrowException(i8* null, %eh.ThrowInfo* null) #11
          to label %105 unwind label %77

; <label>:77                                      ; preds = %76, %74
  %78 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__CxxFrameHandler3 to i8*)
          cleanup
  %79 = extractvalue { i8*, i32 } %78, 0
  store i8* %79, i8** %5
  %80 = extractvalue { i8*, i32 } %78, 1
  store i32 %80, i32* %6
  call void @llvm.eh.endcatch() #5
  call void @llvm.eh.endcatch() #5
  br label %100

; <label>:81                                      ; preds = %69
  call void @llvm.eh.endcatch() #5
  br label %82

; <label>:82                                      ; preds = %81, %56
  %83 = load i32, i32* %1, align 4
  %84 = icmp ult i32 0, %83
  br i1 %84, label %85, label %91

; <label>:85                                      ; preds = %82
  %86 = load i32, i32* %1, align 4
  %87 = bitcast %"class.std::basic_string"* %9 to %"class.std::_String_val"*
  %88 = call x86_thiscallcc i8* @"\01?_Myptr@?$_String_val@U?$_Simple_types@D@std@@@std@@QAEPADXZ"(%"class.std::_String_val"* %87)
  %89 = load i8*, i8** %_Ptr, align 4
  %90 = call i8* @"\01?copy@?$char_traits@D@std@@SAPADPADPBDI@Z"(i8* %89, i8* %88, i32 %86)
  br label %91

; <label>:91                                      ; preds = %85, %82
  call x86_thiscallcc void @"\01?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEX_NI@Z"(%"class.std::basic_string"* %9, i1 zeroext true, i32 0)
  %92 = bitcast %"class.std::basic_string"* %9 to %"class.std::_String_alloc"*
  call x86_thiscallcc void @"\01?_Getal@?$_String_alloc@$0A@U?$_String_base_types@DV?$allocator@D@std@@@std@@@std@@QBE?AU?$_Wrap_alloc@V?$allocator@D@std@@@2@XZ"(%"class.std::_String_alloc"* %92, %"struct.std::_Wrap_alloc"* sret %8)
  %93 = bitcast %"class.std::basic_string"* %9 to %"class.std::_String_val"*
  %94 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %93, i32 0, i32 0
  %95 = bitcast %"union.std::_String_val<std::_Simple_types<char> >::_Bxty"* %94 to i8**
  call x86_thiscallcc void @"\01??$construct@PADAAPAD@?$_Wrap_alloc@V?$allocator@D@std@@@std@@QAEXPAPADAAPAD@Z"(%"struct.std::_Wrap_alloc"* %8, i8** %95, i8** dereferenceable(4) %_Ptr)
  %96 = load i32, i32* %_Newres, align 4
  %97 = bitcast %"class.std::basic_string"* %9 to %"class.std::_String_val"*
  %98 = getelementptr inbounds %"class.std::_String_val", %"class.std::_String_val"* %97, i32 0, i32 2
  store i32 %96, i32* %98, align 4
  %99 = load i32, i32* %1, align 4
  call x86_thiscallcc void @"\01?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEXI@Z"(%"class.std::basic_string"* %9, i32 %99)
  ret void

; <label>:100                                     ; preds = %77
  %101 = load i8*, i8** %5
  %102 = load i32, i32* %6
  %103 = insertvalue { i8*, i32 } undef, i8* %101, 0
  %104 = insertvalue { i8*, i32 } %103, i32 %102, 1
  resume { i8*, i32 } %104

; <label>:105                                     ; preds = %76
  unreachable
}

declare i32 @__CxxFrameHandler3(...)

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc i32 @"\01?max_size@?$_Wrap_alloc@V?$allocator@D@std@@@std@@QBEIXZ"(%"struct.std::_Wrap_alloc"* %this) #3 comdat align 2 {
  %1 = alloca %"struct.std::_Wrap_alloc"*, align 4
  %2 = alloca i8*
  %3 = alloca i32
  store %"struct.std::_Wrap_alloc"* %this, %"struct.std::_Wrap_alloc"** %1, align 4
  %4 = load %"struct.std::_Wrap_alloc"*, %"struct.std::_Wrap_alloc"** %1
  %5 = bitcast %"struct.std::_Wrap_alloc"* %4 to %"class.std::allocator"*
  %6 = invoke i32 @"\01?max_size@?$allocator_traits@V?$allocator@D@std@@@std@@SAIABV?$allocator@D@2@@Z"(%"class.std::allocator"* dereferenceable(1) %5)
          to label %7 unwind label %8

; <label>:7                                       ; preds = %0
  ret i32 %6

; <label>:8                                       ; preds = %0
  %9 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__CxxFrameHandler3 to i8*)
          filter [0 x i8*] zeroinitializer
  %10 = extractvalue { i8*, i32 } %9, 0
  store i8* %10, i8** %2
  %11 = extractvalue { i8*, i32 } %9, 1
  store i32 %11, i32* %3
  br label %12

; <label>:12                                      ; preds = %8
  %13 = load i8*, i8** %2
  call void @__cxa_call_unexpected(i8* %13) #11
  unreachable
}

declare void @__cxa_call_unexpected(i8*)

; Function Attrs: nounwind
define linkonce_odr i32 @"\01?max_size@?$allocator_traits@V?$allocator@D@std@@@std@@SAIABV?$allocator@D@2@@Z"(%"class.std::allocator"* dereferenceable(1) %_Al) #3 comdat align 2 {
  %1 = alloca %"class.std::allocator"*, align 4
  store %"class.std::allocator"* %_Al, %"class.std::allocator"** %1, align 4
  %2 = load %"class.std::allocator"*, %"class.std::allocator"** %1, align 4
  %3 = call x86_thiscallcc i32 @"\01?max_size@?$allocator@D@std@@QBEIXZ"(%"class.std::allocator"* %2) #5
  ret i32 %3
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc i32 @"\01?max_size@?$allocator@D@std@@QBEIXZ"(%"class.std::allocator"* %this) #3 comdat align 2 {
  %1 = alloca %"class.std::allocator"*, align 4
  store %"class.std::allocator"* %this, %"class.std::allocator"** %1, align 4
  %2 = load %"class.std::allocator"*, %"class.std::allocator"** %1
  ret i32 -1
}

; Function Attrs: noreturn
declare void @"\01?_Xlength_error@std@@YAXPBD@Z"(i8*) #2

define linkonce_odr x86_thiscallcc i8* @"\01?allocate@?$_Wrap_alloc@V?$allocator@D@std@@@std@@QAEPADI@Z"(%"struct.std::_Wrap_alloc"* %this, i32 %_Count) #0 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca %"struct.std::_Wrap_alloc"*, align 4
  store i32 %_Count, i32* %1, align 4
  store %"struct.std::_Wrap_alloc"* %this, %"struct.std::_Wrap_alloc"** %2, align 4
  %3 = load %"struct.std::_Wrap_alloc"*, %"struct.std::_Wrap_alloc"** %2
  %4 = bitcast %"struct.std::_Wrap_alloc"* %3 to %"class.std::allocator"*
  %5 = load i32, i32* %1, align 4
  %6 = call x86_thiscallcc i8* @"\01?allocate@?$allocator@D@std@@QAEPADI@Z"(%"class.std::allocator"* %4, i32 %5)
  ret i8* %6
}

; Function Attrs: nounwind
declare void @llvm.eh.begincatch(i8* nocapture, i8* nocapture) #5

declare x86_stdcallcc void @_CxxThrowException(i8*, %eh.ThrowInfo*)

; Function Attrs: nounwind
declare void @llvm.eh.endcatch() #5

define linkonce_odr x86_thiscallcc void @"\01??$construct@PADAAPAD@?$_Wrap_alloc@V?$allocator@D@std@@@std@@QAEXPAPADAAPAD@Z"(%"struct.std::_Wrap_alloc"* %this, i8** %_Ptr, i8** dereferenceable(4) %_Args) #0 comdat align 2 {
  %1 = alloca i8**, align 4
  %2 = alloca i8**, align 4
  %3 = alloca %"struct.std::_Wrap_alloc"*, align 4
  store i8** %_Args, i8*** %1, align 4
  store i8** %_Ptr, i8*** %2, align 4
  store %"struct.std::_Wrap_alloc"* %this, %"struct.std::_Wrap_alloc"** %3, align 4
  %4 = load %"struct.std::_Wrap_alloc"*, %"struct.std::_Wrap_alloc"** %3
  %5 = load i8**, i8*** %1, align 4
  %6 = call dereferenceable(4) i8** @"\01??$forward@AAPAD@std@@YAAAPADAAPAD@Z"(i8** dereferenceable(4) %5)
  %7 = load i8**, i8*** %2, align 4
  %8 = bitcast %"struct.std::_Wrap_alloc"* %4 to %"class.std::allocator"*
  call void @"\01??$construct@PADAAPAD@?$allocator_traits@V?$allocator@D@std@@@std@@SAXAAV?$allocator@D@1@PAPADAAPAD@Z"(%"class.std::allocator"* dereferenceable(1) %8, i8** %7, i8** dereferenceable(4) %6)
  ret void
}

define linkonce_odr x86_thiscallcc i8* @"\01?allocate@?$allocator@D@std@@QAEPADI@Z"(%"class.std::allocator"* %this, i32 %_Count) #0 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca %"class.std::allocator"*, align 4
  store i32 %_Count, i32* %1, align 4
  store %"class.std::allocator"* %this, %"class.std::allocator"** %2, align 4
  %3 = load %"class.std::allocator"*, %"class.std::allocator"** %2
  %4 = load i32, i32* %1, align 4
  %5 = call i8* @"\01??$_Allocate@D@std@@YAPADIPAD@Z"(i32 %4, i8* null)
  ret i8* %5
}

; Function Attrs: inlinehint
define linkonce_odr i8* @"\01??$_Allocate@D@std@@YAPADIPAD@Z"(i32 %_Count, i8*) #7 comdat {
  %2 = alloca i8*, align 4
  %3 = alloca i32, align 4
  %_Ptr = alloca i8*, align 4
  store i8* %0, i8** %2, align 4
  store i32 %_Count, i32* %3, align 4
  store i8* null, i8** %_Ptr, align 4
  %4 = load i32, i32* %3, align 4
  %5 = icmp eq i32 %4, 0
  br i1 %5, label %6, label %7

; <label>:6                                       ; preds = %1
  br label %17

; <label>:7                                       ; preds = %1
  %8 = load i32, i32* %3, align 4
  %9 = icmp ult i32 -1, %8
  br i1 %9, label %15, label %10

; <label>:10                                      ; preds = %7
  %11 = load i32, i32* %3, align 4
  %12 = mul i32 %11, 1
  %13 = call noalias i8* @"\01??2@YAPAXI@Z"(i32 %12)
  store i8* %13, i8** %_Ptr, align 4
  %14 = icmp eq i8* %13, null
  br i1 %14, label %15, label %16

; <label>:15                                      ; preds = %10, %7
  call void @"\01?_Xbad_alloc@std@@YAXXZ"() #11
  unreachable

; <label>:16                                      ; preds = %10
  br label %17

; <label>:17                                      ; preds = %16, %6
  %18 = load i8*, i8** %_Ptr, align 4
  ret i8* %18
}

; Function Attrs: nobuiltin
declare noalias i8* @"\01??2@YAPAXI@Z"(i32) #10

; Function Attrs: noreturn
declare void @"\01?_Xbad_alloc@std@@YAXXZ"() #2

define linkonce_odr void @"\01??$construct@PADAAPAD@?$allocator_traits@V?$allocator@D@std@@@std@@SAXAAV?$allocator@D@1@PAPADAAPAD@Z"(%"class.std::allocator"* dereferenceable(1) %_Al, i8** %_Ptr, i8** dereferenceable(4) %_Args) #0 comdat align 2 {
  %1 = alloca i8**, align 4
  %2 = alloca i8**, align 4
  %3 = alloca %"class.std::allocator"*, align 4
  store i8** %_Args, i8*** %1, align 4
  store i8** %_Ptr, i8*** %2, align 4
  store %"class.std::allocator"* %_Al, %"class.std::allocator"** %3, align 4
  %4 = load %"class.std::allocator"*, %"class.std::allocator"** %3, align 4
  %5 = load i8**, i8*** %1, align 4
  %6 = call dereferenceable(4) i8** @"\01??$forward@AAPAD@std@@YAAAPADAAPAD@Z"(i8** dereferenceable(4) %5)
  %7 = load i8**, i8*** %2, align 4
  call x86_thiscallcc void @"\01??$construct@PADAAPAD@?$allocator@D@std@@QAEXPAPADAAPAD@Z"(%"class.std::allocator"* %4, i8** %7, i8** dereferenceable(4) %6)
  ret void
}

; Function Attrs: inlinehint nounwind
define linkonce_odr dereferenceable(4) i8** @"\01??$forward@AAPAD@std@@YAAAPADAAPAD@Z"(i8** dereferenceable(4) %_Arg) #8 comdat {
  %1 = alloca i8**, align 4
  store i8** %_Arg, i8*** %1, align 4
  %2 = load i8**, i8*** %1, align 4
  ret i8** %2
}

define linkonce_odr x86_thiscallcc void @"\01??$construct@PADAAPAD@?$allocator@D@std@@QAEXPAPADAAPAD@Z"(%"class.std::allocator"* %this, i8** %_Ptr, i8** dereferenceable(4) %_Args) #0 comdat align 2 {
  %1 = alloca i8**, align 4
  %2 = alloca i8**, align 4
  %3 = alloca %"class.std::allocator"*, align 4
  store i8** %_Args, i8*** %1, align 4
  store i8** %_Ptr, i8*** %2, align 4
  store %"class.std::allocator"* %this, %"class.std::allocator"** %3, align 4
  %4 = load %"class.std::allocator"*, %"class.std::allocator"** %3
  %5 = load i8**, i8*** %2, align 4
  %6 = bitcast i8** %5 to i8*
  %7 = bitcast i8* %6 to i8**
  %8 = load i8**, i8*** %1, align 4
  %9 = call dereferenceable(4) i8** @"\01??$forward@AAPAD@std@@YAAAPADAAPAD@Z"(i8** dereferenceable(4) %8)
  %10 = load i8*, i8** %9
  store i8* %10, i8** %7, align 4
  ret void
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc %"class.std::error_condition"* @"\01??0error_condition@std@@QAE@HABVerror_category@1@@Z"(%"class.std::error_condition"* returned %this, i32 %_Val, %"class.std::error_category"* dereferenceable(4) %_Cat) unnamed_addr #3 comdat align 2 {
  %1 = alloca %"class.std::error_category"*, align 4
  %2 = alloca i32, align 4
  %3 = alloca %"class.std::error_condition"*, align 4
  store %"class.std::error_category"* %_Cat, %"class.std::error_category"** %1, align 4
  store i32 %_Val, i32* %2, align 4
  store %"class.std::error_condition"* %this, %"class.std::error_condition"** %3, align 4
  %4 = load %"class.std::error_condition"*, %"class.std::error_condition"** %3
  %5 = getelementptr inbounds %"class.std::error_condition", %"class.std::error_condition"* %4, i32 0, i32 0
  %6 = load i32, i32* %2, align 4
  store i32 %6, i32* %5, align 4
  %7 = getelementptr inbounds %"class.std::error_condition", %"class.std::error_condition"* %4, i32 0, i32 1
  %8 = load %"class.std::error_category"*, %"class.std::error_category"** %1, align 4
  store %"class.std::error_category"* %8, %"class.std::error_category"** %7, align 4
  ret %"class.std::error_condition"* %4
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc zeroext i1 @"\01??8error_category@std@@QBE_NABV01@@Z"(%"class.std::error_category"* %this, %"class.std::error_category"* dereferenceable(4) %_Right) #3 comdat align 2 {
  %1 = alloca %"class.std::error_category"*, align 4
  %2 = alloca %"class.std::error_category"*, align 4
  store %"class.std::error_category"* %_Right, %"class.std::error_category"** %1, align 4
  store %"class.std::error_category"* %this, %"class.std::error_category"** %2, align 4
  %3 = load %"class.std::error_category"*, %"class.std::error_category"** %2
  %4 = load %"class.std::error_category"*, %"class.std::error_category"** %1, align 4
  %5 = icmp eq %"class.std::error_category"* %3, %4
  ret i1 %5
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc dereferenceable(4) %"class.std::error_category"* @"\01?category@error_code@std@@QBEABVerror_category@2@XZ"(%"class.std::error_code"* %this) #3 comdat align 2 {
  %1 = alloca %"class.std::error_code"*, align 4
  store %"class.std::error_code"* %this, %"class.std::error_code"** %1, align 4
  %2 = load %"class.std::error_code"*, %"class.std::error_code"** %1
  %3 = getelementptr inbounds %"class.std::error_code", %"class.std::error_code"* %2, i32 0, i32 1
  %4 = load %"class.std::error_category"*, %"class.std::error_category"** %3, align 4
  ret %"class.std::error_category"* %4
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc i32 @"\01?value@error_code@std@@QBEHXZ"(%"class.std::error_code"* %this) #3 comdat align 2 {
  %1 = alloca %"class.std::error_code"*, align 4
  store %"class.std::error_code"* %this, %"class.std::error_code"** %1, align 4
  %2 = load %"class.std::error_code"*, %"class.std::error_code"** %1
  %3 = getelementptr inbounds %"class.std::error_code", %"class.std::error_code"* %2, i32 0, i32 0
  %4 = load i32, i32* %3, align 4
  ret i32 %4
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc zeroext i1 @"\01??8error_condition@std@@QBE_NABV01@@Z"(%"class.std::error_condition"* %this, %"class.std::error_condition"* dereferenceable(8) %_Right) #3 comdat align 2 {
  %1 = alloca %"class.std::error_condition"*, align 4
  %2 = alloca %"class.std::error_condition"*, align 4
  store %"class.std::error_condition"* %_Right, %"class.std::error_condition"** %1, align 4
  store %"class.std::error_condition"* %this, %"class.std::error_condition"** %2, align 4
  %3 = load %"class.std::error_condition"*, %"class.std::error_condition"** %2
  %4 = call x86_thiscallcc dereferenceable(4) %"class.std::error_category"* @"\01?category@error_condition@std@@QBEABVerror_category@2@XZ"(%"class.std::error_condition"* %3) #5
  %5 = load %"class.std::error_condition"*, %"class.std::error_condition"** %1, align 4
  %6 = call x86_thiscallcc dereferenceable(4) %"class.std::error_category"* @"\01?category@error_condition@std@@QBEABVerror_category@2@XZ"(%"class.std::error_condition"* %5) #5
  %7 = call x86_thiscallcc zeroext i1 @"\01??8error_category@std@@QBE_NABV01@@Z"(%"class.std::error_category"* %4, %"class.std::error_category"* dereferenceable(4) %6) #5
  br i1 %7, label %8, label %13

; <label>:8                                       ; preds = %0
  %9 = call x86_thiscallcc i32 @"\01?value@error_condition@std@@QBEHXZ"(%"class.std::error_condition"* %3) #5
  %10 = load %"class.std::error_condition"*, %"class.std::error_condition"** %1, align 4
  %11 = call x86_thiscallcc i32 @"\01?value@error_condition@std@@QBEHXZ"(%"class.std::error_condition"* %10) #5
  %12 = icmp eq i32 %9, %11
  br label %13

; <label>:13                                      ; preds = %8, %0
  %14 = phi i1 [ false, %0 ], [ %12, %8 ]
  ret i1 %14
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc dereferenceable(4) %"class.std::error_category"* @"\01?category@error_condition@std@@QBEABVerror_category@2@XZ"(%"class.std::error_condition"* %this) #3 comdat align 2 {
  %1 = alloca %"class.std::error_condition"*, align 4
  store %"class.std::error_condition"* %this, %"class.std::error_condition"** %1, align 4
  %2 = load %"class.std::error_condition"*, %"class.std::error_condition"** %1
  %3 = getelementptr inbounds %"class.std::error_condition", %"class.std::error_condition"* %2, i32 0, i32 1
  %4 = load %"class.std::error_category"*, %"class.std::error_category"** %3, align 4
  ret %"class.std::error_category"* %4
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc i32 @"\01?value@error_condition@std@@QBEHXZ"(%"class.std::error_condition"* %this) #3 comdat align 2 {
  %1 = alloca %"class.std::error_condition"*, align 4
  store %"class.std::error_condition"* %this, %"class.std::error_condition"** %1, align 4
  %2 = load %"class.std::error_condition"*, %"class.std::error_condition"** %1
  %3 = getelementptr inbounds %"class.std::error_condition", %"class.std::error_condition"* %2, i32 0, i32 0
  %4 = load i32, i32* %3, align 4
  ret i32 %4
}

; Function Attrs: inlinehint nounwind
define linkonce_odr x86_thiscallcc i8* @"\01??_G_Iostream_error_category@std@@UAEPAXI@Z"(%"class.std::_Iostream_error_category"* %this, i32 %should_call_delete) unnamed_addr #8 comdat align 2 {
  %1 = alloca i8*, align 4
  %2 = alloca i32, align 4
  %3 = alloca %"class.std::_Iostream_error_category"*, align 4
  store i32 %should_call_delete, i32* %2, align 4
  store %"class.std::_Iostream_error_category"* %this, %"class.std::_Iostream_error_category"** %3, align 4
  %4 = load %"class.std::_Iostream_error_category"*, %"class.std::_Iostream_error_category"** %3
  %5 = bitcast %"class.std::_Iostream_error_category"* %4 to i8*
  store i8* %5, i8** %1
  %6 = load i32, i32* %2
  call x86_thiscallcc void @"\01??1_Iostream_error_category@std@@UAE@XZ"(%"class.std::_Iostream_error_category"* %4) #5
  %7 = icmp eq i32 %6, 0
  br i1 %7, label %10, label %8

; <label>:8                                       ; preds = %0
  %9 = bitcast %"class.std::_Iostream_error_category"* %4 to i8*
  call void @"\01??3@YAXPAX@Z"(i8* %9) #13
  br label %10

; <label>:10                                      ; preds = %8, %0
  %11 = load i8*, i8** %1
  ret i8* %11
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc i8* @"\01?name@_Iostream_error_category@std@@UBEPBDXZ"(%"class.std::_Iostream_error_category"* %this) unnamed_addr #3 comdat align 2 {
  %1 = alloca %"class.std::_Iostream_error_category"*, align 4
  store %"class.std::_Iostream_error_category"* %this, %"class.std::_Iostream_error_category"** %1, align 4
  %2 = load %"class.std::_Iostream_error_category"*, %"class.std::_Iostream_error_category"** %1
  ret i8* getelementptr inbounds ([9 x i8], [9 x i8]* @"\01??_C@_08LLGCOLLL@iostream?$AA@", i32 0, i32 0)
}

define linkonce_odr x86_thiscallcc void @"\01?message@_Iostream_error_category@std@@UBE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@2@H@Z"(%"class.std::_Iostream_error_category"* %this, %"class.std::basic_string"* noalias sret %agg.result, i32 %_Errcode) unnamed_addr #0 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca %"class.std::_Iostream_error_category"*, align 4
  store i32 %_Errcode, i32* %1, align 4
  store %"class.std::_Iostream_error_category"* %this, %"class.std::_Iostream_error_category"** %2, align 4
  %3 = load %"class.std::_Iostream_error_category"*, %"class.std::_Iostream_error_category"** %2
  %4 = load i32, i32* %1, align 4
  %5 = icmp eq i32 %4, 1
  br i1 %5, label %6, label %8

; <label>:6                                       ; preds = %0
  %7 = call x86_thiscallcc %"class.std::basic_string"* @"\01??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z"(%"class.std::basic_string"* %agg.result, i8* getelementptr inbounds ([22 x i8], [22 x i8]* @"\01??_C@_0BG@PADBLCHM@iostream?5stream?5error?$AA@", i32 0, i32 0))
  br label %11

; <label>:8                                       ; preds = %0
  %9 = bitcast %"class.std::_Iostream_error_category"* %3 to %"class.std::_Generic_error_category"*
  %10 = load i32, i32* %1, align 4
  call x86_thiscallcc void @"\01?message@_Generic_error_category@std@@UBE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@2@H@Z"(%"class.std::_Generic_error_category"* %9, %"class.std::basic_string"* sret %agg.result, i32 %10)
  br label %11

; <label>:11                                      ; preds = %8, %6
  ret void
}

; Function Attrs: inlinehint nounwind
define linkonce_odr x86_thiscallcc i8* @"\01??_G_System_error_category@std@@UAEPAXI@Z"(%"class.std::_System_error_category"* %this, i32 %should_call_delete) unnamed_addr #8 comdat align 2 {
  %1 = alloca i8*, align 4
  %2 = alloca i32, align 4
  %3 = alloca %"class.std::_System_error_category"*, align 4
  store i32 %should_call_delete, i32* %2, align 4
  store %"class.std::_System_error_category"* %this, %"class.std::_System_error_category"** %3, align 4
  %4 = load %"class.std::_System_error_category"*, %"class.std::_System_error_category"** %3
  %5 = bitcast %"class.std::_System_error_category"* %4 to i8*
  store i8* %5, i8** %1
  %6 = load i32, i32* %2
  call x86_thiscallcc void @"\01??1_System_error_category@std@@UAE@XZ"(%"class.std::_System_error_category"* %4) #5
  %7 = icmp eq i32 %6, 0
  br i1 %7, label %10, label %8

; <label>:8                                       ; preds = %0
  %9 = bitcast %"class.std::_System_error_category"* %4 to i8*
  call void @"\01??3@YAXPAX@Z"(i8* %9) #13
  br label %10

; <label>:10                                      ; preds = %8, %0
  %11 = load i8*, i8** %1
  ret i8* %11
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc i8* @"\01?name@_System_error_category@std@@UBEPBDXZ"(%"class.std::_System_error_category"* %this) unnamed_addr #3 comdat align 2 {
  %1 = alloca %"class.std::_System_error_category"*, align 4
  store %"class.std::_System_error_category"* %this, %"class.std::_System_error_category"** %1, align 4
  %2 = load %"class.std::_System_error_category"*, %"class.std::_System_error_category"** %1
  ret i8* getelementptr inbounds ([7 x i8], [7 x i8]* @"\01??_C@_06FHFOAHML@system?$AA@", i32 0, i32 0)
}

define linkonce_odr x86_thiscallcc void @"\01?message@_System_error_category@std@@UBE?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@2@H@Z"(%"class.std::_System_error_category"* %this, %"class.std::basic_string"* noalias sret %agg.result, i32 %_Errcode) unnamed_addr #0 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca %"class.std::_System_error_category"*, align 4
  %_Name = alloca i8*, align 4
  store i32 %_Errcode, i32* %1, align 4
  store %"class.std::_System_error_category"* %this, %"class.std::_System_error_category"** %2, align 4
  %3 = load %"class.std::_System_error_category"*, %"class.std::_System_error_category"** %2
  %4 = load i32, i32* %1, align 4
  %5 = call i8* @"\01?_Winerror_map@std@@YAPBDH@Z"(i32 %4)
  store i8* %5, i8** %_Name, align 4
  %6 = load i8*, i8** %_Name, align 4
  %7 = icmp ne i8* %6, null
  br i1 %7, label %8, label %10

; <label>:8                                       ; preds = %0
  %9 = load i8*, i8** %_Name, align 4
  br label %11

; <label>:10                                      ; preds = %0
  br label %11

; <label>:11                                      ; preds = %10, %8
  %12 = phi i8* [ %9, %8 ], [ getelementptr inbounds ([14 x i8], [14 x i8]* @"\01??_C@_0O@BFJCFAAK@unknown?5error?$AA@", i32 0, i32 0), %10 ]
  %13 = call x86_thiscallcc %"class.std::basic_string"* @"\01??0?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAE@PBD@Z"(%"class.std::basic_string"* %agg.result, i8* %12)
  ret void
}

; Function Attrs: nounwind
define linkonce_odr x86_thiscallcc void @"\01?default_error_condition@_System_error_category@std@@UBE?AVerror_condition@2@H@Z"(%"class.std::_System_error_category"* %this, %"class.std::error_condition"* noalias sret %agg.result, i32 %_Errval) unnamed_addr #3 comdat align 2 {
  %1 = alloca i32, align 4
  %2 = alloca %"class.std::_System_error_category"*, align 4
  %3 = alloca i8*
  %4 = alloca i32
  store i32 %_Errval, i32* %1, align 4
  store %"class.std::_System_error_category"* %this, %"class.std::_System_error_category"** %2, align 4
  %5 = load %"class.std::_System_error_category"*, %"class.std::_System_error_category"** %2
  %6 = load i32, i32* %1, align 4
  %7 = invoke i8* @"\01?_Syserror_map@std@@YAPBDH@Z"(i32 %6)
          to label %8 unwind label %14

; <label>:8                                       ; preds = %0
  %9 = icmp ne i8* %7, null
  br i1 %9, label %10, label %20

; <label>:10                                      ; preds = %8
  %11 = call dereferenceable(4) %"class.std::error_category"* @"\01?generic_category@std@@YAABVerror_category@1@XZ"() #5
  %12 = load i32, i32* %1, align 4
  %13 = call x86_thiscallcc %"class.std::error_condition"* @"\01??0error_condition@std@@QAE@HABVerror_category@1@@Z"(%"class.std::error_condition"* %agg.result, i32 %12, %"class.std::error_category"* dereferenceable(4) %11) #5
  br label %24

; <label>:14                                      ; preds = %0
  %15 = landingpad { i8*, i32 } personality i8* bitcast (i32 (...)* @__CxxFrameHandler3 to i8*)
          filter [0 x i8*] zeroinitializer
  %16 = extractvalue { i8*, i32 } %15, 0
  store i8* %16, i8** %3
  %17 = extractvalue { i8*, i32 } %15, 1
  store i32 %17, i32* %4
  br label %18

; <label>:18                                      ; preds = %14
  %19 = load i8*, i8** %3
  call void @__cxa_call_unexpected(i8* %19) #11
  unreachable

; <label>:20                                      ; preds = %8
  %21 = call dereferenceable(4) %"class.std::error_category"* @"\01?system_category@std@@YAABVerror_category@1@XZ"() #5
  %22 = load i32, i32* %1, align 4
  %23 = call x86_thiscallcc %"class.std::error_condition"* @"\01??0error_condition@std@@QAE@HABVerror_category@1@@Z"(%"class.std::error_condition"* %agg.result, i32 %22, %"class.std::error_category"* dereferenceable(4) %21) #5
  br label %24

; <label>:24                                      ; preds = %20, %10
  ret void
}

declare i8* @"\01?_Winerror_map@std@@YAPBDH@Z"(i32) #0

; Function Attrs: inlinehint nounwind
define linkonce_odr dereferenceable(4) %"class.std::error_category"* @"\01?generic_category@std@@YAABVerror_category@1@XZ"() #8 comdat {
  ret %"class.std::error_category"* getelementptr inbounds (%"class.std::_Generic_error_category", %"class.std::_Generic_error_category"* @"\01?_Generic_object@?$_Error_objects@H@std@@2V_Generic_error_category@2@A", i32 0, i32 0)
}

; Function Attrs: inlinehint nounwind
define linkonce_odr dereferenceable(4) %"class.std::error_category"* @"\01?system_category@std@@YAABVerror_category@1@XZ"() #8 comdat {
  ret %"class.std::error_category"* getelementptr inbounds (%"class.std::_System_error_category", %"class.std::_System_error_category"* @"\01?_System_object@?$_Error_objects@H@std@@2V_System_error_category@2@A", i32 0, i32 0, i32 0)
}

attributes #0 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { returns_twice }
attributes #2 = { noreturn "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { noreturn nounwind }
attributes #5 = { nounwind }
attributes #6 = { nounwind readnone "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #7 = { inlinehint "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #8 = { inlinehint nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #9 = { nobuiltin nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #10 = { nobuiltin "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #11 = { noreturn }
attributes #12 = { nounwind readnone }
attributes #13 = { builtin nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!8}

!0 = !{i32 6, !"Linker Options", !1}
!1 = !{!2, !2, !3, !4, !5, !6, !7}
!2 = !{!"/DEFAULTLIB:uuid.lib"}
!3 = !{!"/FAILIFMISMATCH:\22_MSC_VER=1800\22"}
!4 = !{!"/FAILIFMISMATCH:\22_ITERATOR_DEBUG_LEVEL=0\22"}
!5 = !{!"/FAILIFMISMATCH:\22RuntimeLibrary=MT_StaticRelease\22"}
!6 = !{!"/DEFAULTLIB:libcpmt.lib"}
!7 = !{!"/DEFAULTLIB:Winmm.lib"}
!8 = !{!"clang version 3.7.0 (trunk)"}
