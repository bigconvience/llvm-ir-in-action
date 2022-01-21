; ModuleID = 'hello.cpp'
source_filename = "hello.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%"class.std::ios_base::Init" = type { i8 }
%"class.std::basic_ostream" = type { i32 (...)**, %"class.std::basic_ios" }
%"class.std::basic_ios" = type { %"class.std::ios_base", %"class.std::basic_ostream"*, i8, i8, %"class.std::basic_streambuf"*, %"class.std::ctype"*, %"class.std::num_put"*, %"class.std::num_get"* }
%"class.std::ios_base" = type { i32 (...)**, i64, i64, i32, i32, i32, %"struct.std::ios_base::_Callback_list"*, %"struct.std::ios_base::_Words", [8 x %"struct.std::ios_base::_Words"], i32, %"struct.std::ios_base::_Words"*, %"class.std::locale" }
%"struct.std::ios_base::_Callback_list" = type { %"struct.std::ios_base::_Callback_list"*, void (i32, %"class.std::ios_base"*, i32)*, i32, i32 }
%"struct.std::ios_base::_Words" = type { i8*, i64 }
%"class.std::locale" = type { %"class.std::locale::_Impl"* }
%"class.std::locale::_Impl" = type { i32, %"class.std::locale::facet"**, i64, %"class.std::locale::facet"**, i8** }
%"class.std::locale::facet" = type <{ i32 (...)**, i32, [4 x i8] }>
%"class.std::basic_streambuf" = type { i32 (...)**, i8*, i8*, i8*, i8*, i8*, i8*, %"class.std::locale" }
%"class.std::ctype" = type <{ %"class.std::locale::facet.base", [4 x i8], %struct.__locale_struct*, i8, [7 x i8], i32*, i32*, i16*, i8, [256 x i8], [256 x i8], i8, [6 x i8] }>
%"class.std::locale::facet.base" = type <{ i32 (...)**, i32 }>
%struct.__locale_struct = type { [13 x %struct.__locale_data*], i16*, i32*, i32*, [13 x i8*] }
%struct.__locale_data = type opaque
%"class.std::num_put" = type { %"class.std::locale::facet.base", [4 x i8] }
%"class.std::num_get" = type { %"class.std::locale::facet.base", [4 x i8] }
%class.Rectangle = type { %class.Square, double }
%class.Square = type { i32 (...)**, double }

$_ZN9RectangleC2Ev = comdat any

$_ZN6Square3setEd = comdat any

$_ZN9Rectangle9setLengthEd = comdat any

$_ZN6SquareC2Ev = comdat any

$_ZN9Rectangle7getAreaEv = comdat any

$_ZN6Square7getAreaEv = comdat any

$_ZTV9Rectangle = comdat any

$_ZTS9Rectangle = comdat any

$_ZTS6Square = comdat any

$_ZTI6Square = comdat any

$_ZTI9Rectangle = comdat any

$_ZTV6Square = comdat any

@_ZStL8__ioinit = internal global %"class.std::ios_base::Init" zeroinitializer, align 1
@__dso_handle = external hidden global i8
@_ZSt4cout = external dso_local global %"class.std::basic_ostream", align 8
@.str = private unnamed_addr constant [10 x i8] c"rec area \00", align 1
@_ZTV9Rectangle = linkonce_odr dso_local unnamed_addr constant { [3 x i8*] } { [3 x i8*] [i8* null, i8* bitcast ({ i8*, i8*, i8* }* @_ZTI9Rectangle to i8*), i8* bitcast (double (%class.Rectangle*)* @_ZN9Rectangle7getAreaEv to i8*)] }, comdat, align 8
@_ZTVN10__cxxabiv120__si_class_type_infoE = external dso_local global i8*
@_ZTS9Rectangle = linkonce_odr dso_local constant [11 x i8] c"9Rectangle\00", comdat, align 1
@_ZTVN10__cxxabiv117__class_type_infoE = external dso_local global i8*
@_ZTS6Square = linkonce_odr dso_local constant [8 x i8] c"6Square\00", comdat, align 1
@_ZTI6Square = linkonce_odr dso_local constant { i8*, i8* } { i8* bitcast (i8** getelementptr inbounds (i8*, i8** @_ZTVN10__cxxabiv117__class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([8 x i8], [8 x i8]* @_ZTS6Square, i32 0, i32 0) }, comdat, align 8
@_ZTI9Rectangle = linkonce_odr dso_local constant { i8*, i8*, i8* } { i8* bitcast (i8** getelementptr inbounds (i8*, i8** @_ZTVN10__cxxabiv120__si_class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([11 x i8], [11 x i8]* @_ZTS9Rectangle, i32 0, i32 0), i8* bitcast ({ i8*, i8* }* @_ZTI6Square to i8*) }, comdat, align 8
@_ZTV6Square = linkonce_odr dso_local unnamed_addr constant { [3 x i8*] } { [3 x i8*] [i8* null, i8* bitcast ({ i8*, i8* }* @_ZTI6Square to i8*), i8* bitcast (double (%class.Square*)* @_ZN6Square7getAreaEv to i8*)] }, comdat, align 8
@llvm.global_ctors = appending global [1 x { i32, void ()*, i8* }] [{ i32, void ()*, i8* } { i32 65535, void ()* @_GLOBAL__sub_I_hello.cpp, i8* null }]

; Function Attrs: noinline uwtable
define internal void @__cxx_global_var_init() #0 section ".text.startup" {
  call void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* nonnull align 1 dereferenceable(1) @_ZStL8__ioinit)
  %1 = call i32 @__cxa_atexit(void (i8*)* bitcast (void (%"class.std::ios_base::Init"*)* @_ZNSt8ios_base4InitD1Ev to void (i8*)*), i8* getelementptr inbounds (%"class.std::ios_base::Init", %"class.std::ios_base::Init"* @_ZStL8__ioinit, i32 0, i32 0), i8* @__dso_handle) #3
  ret void
}

declare dso_local void @_ZNSt8ios_base4InitC1Ev(%"class.std::ios_base::Init"* nonnull align 1 dereferenceable(1)) unnamed_addr #1

; Function Attrs: nounwind
declare dso_local void @_ZNSt8ios_base4InitD1Ev(%"class.std::ios_base::Init"* nonnull align 1 dereferenceable(1)) unnamed_addr #2

; Function Attrs: nounwind
declare dso_local i32 @__cxa_atexit(void (i8*)*, i8*, i8*) #3

; Function Attrs: mustprogress noinline norecurse optnone uwtable
define dso_local i32 @main() #4 {
  %1 = alloca i32, align 4
  %2 = alloca %class.Rectangle, align 8
  %3 = alloca %class.Rectangle*, align 8
  %4 = alloca double, align 8
  store i32 0, i32* %1, align 4
  call void @_ZN9RectangleC2Ev(%class.Rectangle* nonnull align 8 dereferenceable(24) %2) #3
  store %class.Rectangle* %2, %class.Rectangle** %3, align 8
  %5 = load %class.Rectangle*, %class.Rectangle** %3, align 8
  %6 = bitcast %class.Rectangle* %5 to %class.Square*
  call void @_ZN6Square3setEd(%class.Square* nonnull align 8 dereferenceable(16) %6, double 3.000000e+00)
  %7 = load %class.Rectangle*, %class.Rectangle** %3, align 8
  call void @_ZN9Rectangle9setLengthEd(%class.Rectangle* nonnull align 8 dereferenceable(24) %7, double 4.000000e+00)
  %8 = load %class.Rectangle*, %class.Rectangle** %3, align 8
  %9 = bitcast %class.Rectangle* %8 to double (%class.Rectangle*)***
  %10 = load double (%class.Rectangle*)**, double (%class.Rectangle*)*** %9, align 8
  %11 = getelementptr inbounds double (%class.Rectangle*)*, double (%class.Rectangle*)** %10, i64 0
  %12 = load double (%class.Rectangle*)*, double (%class.Rectangle*)** %11, align 8
  %13 = call double %12(%class.Rectangle* nonnull align 8 dereferenceable(24) %8)
  store double %13, double* %4, align 8
  %14 = call nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* nonnull align 8 dereferenceable(8) @_ZSt4cout, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i64 0, i64 0))
  %15 = load double, double* %4, align 8
  %16 = call nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZNSolsEd(%"class.std::basic_ostream"* nonnull align 8 dereferenceable(8) %14, double %15)
  %17 = call nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"* nonnull align 8 dereferenceable(8) %16, %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)* @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_)
  ret i32 0
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN9RectangleC2Ev(%class.Rectangle* nonnull align 8 dereferenceable(24) %0) unnamed_addr #5 comdat align 2 {
  %2 = alloca %class.Rectangle*, align 8
  store %class.Rectangle* %0, %class.Rectangle** %2, align 8
  %3 = load %class.Rectangle*, %class.Rectangle** %2, align 8
  %4 = bitcast %class.Rectangle* %3 to %class.Square*
  call void @_ZN6SquareC2Ev(%class.Square* nonnull align 8 dereferenceable(16) %4) #3
  %5 = bitcast %class.Rectangle* %3 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ({ [3 x i8*] }, { [3 x i8*] }* @_ZTV9Rectangle, i32 0, inrange i32 0, i32 2) to i32 (...)**), i32 (...)*** %5, align 8
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN6Square3setEd(%class.Square* nonnull align 8 dereferenceable(16) %0, double %1) #6 comdat align 2 {
  %3 = alloca %class.Square*, align 8
  %4 = alloca double, align 8
  store %class.Square* %0, %class.Square** %3, align 8
  store double %1, double* %4, align 8
  %5 = load %class.Square*, %class.Square** %3, align 8
  %6 = load double, double* %4, align 8
  %7 = getelementptr inbounds %class.Square, %class.Square* %5, i32 0, i32 1
  store double %6, double* %7, align 8
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN9Rectangle9setLengthEd(%class.Rectangle* nonnull align 8 dereferenceable(24) %0, double %1) #6 comdat align 2 {
  %3 = alloca %class.Rectangle*, align 8
  %4 = alloca double, align 8
  store %class.Rectangle* %0, %class.Rectangle** %3, align 8
  store double %1, double* %4, align 8
  %5 = load %class.Rectangle*, %class.Rectangle** %3, align 8
  %6 = load double, double* %4, align 8
  %7 = getelementptr inbounds %class.Rectangle, %class.Rectangle* %5, i32 0, i32 1
  store double %6, double* %7, align 8
  ret void
}

declare dso_local nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc(%"class.std::basic_ostream"* nonnull align 8 dereferenceable(8), i8*) #1

declare dso_local nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZNSolsEd(%"class.std::basic_ostream"* nonnull align 8 dereferenceable(8), double) #1

declare dso_local nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZNSolsEPFRSoS_E(%"class.std::basic_ostream"* nonnull align 8 dereferenceable(8), %"class.std::basic_ostream"* (%"class.std::basic_ostream"*)*) #1

declare dso_local nonnull align 8 dereferenceable(8) %"class.std::basic_ostream"* @_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_(%"class.std::basic_ostream"* nonnull align 8 dereferenceable(8)) #1

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN6SquareC2Ev(%class.Square* nonnull align 8 dereferenceable(16) %0) unnamed_addr #5 comdat align 2 {
  %2 = alloca %class.Square*, align 8
  store %class.Square* %0, %class.Square** %2, align 8
  %3 = load %class.Square*, %class.Square** %2, align 8
  %4 = bitcast %class.Square* %3 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ({ [3 x i8*] }, { [3 x i8*] }* @_ZTV6Square, i32 0, inrange i32 0, i32 2) to i32 (...)**), i32 (...)*** %4, align 8
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr dso_local double @_ZN9Rectangle7getAreaEv(%class.Rectangle* nonnull align 8 dereferenceable(24) %0) unnamed_addr #6 comdat align 2 {
  %2 = alloca %class.Rectangle*, align 8
  store %class.Rectangle* %0, %class.Rectangle** %2, align 8
  %3 = load %class.Rectangle*, %class.Rectangle** %2, align 8
  %4 = bitcast %class.Rectangle* %3 to %class.Square*
  %5 = getelementptr inbounds %class.Square, %class.Square* %4, i32 0, i32 1
  %6 = load double, double* %5, align 8
  %7 = getelementptr inbounds %class.Rectangle, %class.Rectangle* %3, i32 0, i32 1
  %8 = load double, double* %7, align 8
  %9 = fmul double %6, %8
  ret double %9
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr dso_local double @_ZN6Square7getAreaEv(%class.Square* nonnull align 8 dereferenceable(16) %0) unnamed_addr #6 comdat align 2 {
  %2 = alloca %class.Square*, align 8
  store %class.Square* %0, %class.Square** %2, align 8
  %3 = load %class.Square*, %class.Square** %2, align 8
  %4 = getelementptr inbounds %class.Square, %class.Square* %3, i32 0, i32 1
  %5 = load double, double* %4, align 8
  %6 = getelementptr inbounds %class.Square, %class.Square* %3, i32 0, i32 1
  %7 = load double, double* %6, align 8
  %8 = fmul double %5, %7
  ret double %8
}

; Function Attrs: noinline uwtable
define internal void @_GLOBAL__sub_I_hello.cpp() #0 section ".text.startup" {
  call void @__cxx_global_var_init()
  ret void
}

attributes #0 = { noinline uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind }
attributes #4 = { mustprogress noinline norecurse optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { mustprogress noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"Ubuntu clang version 13.0.1-++20211217022950+e68f640deecc-1~exp1~20211217023024.30"}
