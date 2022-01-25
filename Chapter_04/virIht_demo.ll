; ModuleID = 'virIht_demo.cpp'
source_filename = "virIht_demo.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%class.Derived = type { %class.BaseB.base, [4 x i8], %class.BaseC.base, i32, %class.BaseA, [4 x i8] }
%class.BaseB.base = type <{ i32 (...)**, i32 }>
%class.BaseC.base = type <{ i32 (...)**, i32 }>
%class.BaseA = type { i32 }
%class.BaseB = type <{ i32 (...)**, i32, %class.BaseA }>
%class.BaseC = type <{ i32 (...)**, i32, %class.BaseA }>

$_ZN7DerivedC1Ev = comdat any

$_ZN7Derived6sumDerEv = comdat any

$_ZN5BaseBC2Ev = comdat any

$_ZN5BaseCC2Ev = comdat any

$_ZN5BaseB4sumBEv = comdat any

$_ZN5BaseC4sumCEv = comdat any

$_ZTV7Derived = comdat any

$_ZTT7Derived = comdat any

$_ZTC7Derived0_5BaseB = comdat any

$_ZTS5BaseB = comdat any

$_ZTS5BaseA = comdat any

$_ZTI5BaseA = comdat any

$_ZTI5BaseB = comdat any

$_ZTC7Derived16_5BaseC = comdat any

$_ZTS5BaseC = comdat any

$_ZTI5BaseC = comdat any

$_ZTS7Derived = comdat any

$_ZTI7Derived = comdat any

@_ZTV7Derived = linkonce_odr dso_local unnamed_addr constant { [3 x i8*], [3 x i8*] } { [3 x i8*] [i8* inttoptr (i64 32 to i8*), i8* null, i8* bitcast ({ i8*, i8*, i32, i32, i8*, i64, i8*, i64 }* @_ZTI7Derived to i8*)], [3 x i8*] [i8* inttoptr (i64 16 to i8*), i8* inttoptr (i64 -16 to i8*), i8* bitcast ({ i8*, i8*, i32, i32, i8*, i64, i8*, i64 }* @_ZTI7Derived to i8*)] }, comdat, align 8
@_ZTT7Derived = linkonce_odr dso_local unnamed_addr constant [4 x i8*] [i8* bitcast (i8** getelementptr inbounds ({ [3 x i8*], [3 x i8*] }, { [3 x i8*], [3 x i8*] }* @_ZTV7Derived, i32 0, inrange i32 0, i32 3) to i8*), i8* bitcast (i8** getelementptr inbounds ({ [3 x i8*] }, { [3 x i8*] }* @_ZTC7Derived0_5BaseB, i32 0, inrange i32 0, i32 3) to i8*), i8* bitcast (i8** getelementptr inbounds ({ [3 x i8*] }, { [3 x i8*] }* @_ZTC7Derived16_5BaseC, i32 0, inrange i32 0, i32 3) to i8*), i8* bitcast (i8** getelementptr inbounds ({ [3 x i8*], [3 x i8*] }, { [3 x i8*], [3 x i8*] }* @_ZTV7Derived, i32 0, inrange i32 1, i32 3) to i8*)], comdat, align 8
@_ZTC7Derived0_5BaseB = linkonce_odr dso_local unnamed_addr constant { [3 x i8*] } { [3 x i8*] [i8* inttoptr (i64 32 to i8*), i8* null, i8* bitcast ({ i8*, i8*, i32, i32, i8*, i64 }* @_ZTI5BaseB to i8*)] }, comdat, align 8
@_ZTVN10__cxxabiv121__vmi_class_type_infoE = external dso_local global i8*
@_ZTS5BaseB = linkonce_odr dso_local constant [7 x i8] c"5BaseB\00", comdat, align 1
@_ZTVN10__cxxabiv117__class_type_infoE = external dso_local global i8*
@_ZTS5BaseA = linkonce_odr dso_local constant [7 x i8] c"5BaseA\00", comdat, align 1
@_ZTI5BaseA = linkonce_odr dso_local constant { i8*, i8* } { i8* bitcast (i8** getelementptr inbounds (i8*, i8** @_ZTVN10__cxxabiv117__class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @_ZTS5BaseA, i32 0, i32 0) }, comdat, align 8
@_ZTI5BaseB = linkonce_odr dso_local constant { i8*, i8*, i32, i32, i8*, i64 } { i8* bitcast (i8** getelementptr inbounds (i8*, i8** @_ZTVN10__cxxabiv121__vmi_class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @_ZTS5BaseB, i32 0, i32 0), i32 0, i32 1, i8* bitcast ({ i8*, i8* }* @_ZTI5BaseA to i8*), i64 -6143 }, comdat, align 8
@_ZTC7Derived16_5BaseC = linkonce_odr dso_local unnamed_addr constant { [3 x i8*] } { [3 x i8*] [i8* inttoptr (i64 16 to i8*), i8* null, i8* bitcast ({ i8*, i8*, i32, i32, i8*, i64 }* @_ZTI5BaseC to i8*)] }, comdat, align 8
@_ZTS5BaseC = linkonce_odr dso_local constant [7 x i8] c"5BaseC\00", comdat, align 1
@_ZTI5BaseC = linkonce_odr dso_local constant { i8*, i8*, i32, i32, i8*, i64 } { i8* bitcast (i8** getelementptr inbounds (i8*, i8** @_ZTVN10__cxxabiv121__vmi_class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([7 x i8], [7 x i8]* @_ZTS5BaseC, i32 0, i32 0), i32 0, i32 1, i8* bitcast ({ i8*, i8* }* @_ZTI5BaseA to i8*), i64 -6143 }, comdat, align 8
@_ZTS7Derived = linkonce_odr dso_local constant [9 x i8] c"7Derived\00", comdat, align 1
@_ZTI7Derived = linkonce_odr dso_local constant { i8*, i8*, i32, i32, i8*, i64, i8*, i64 } { i8* bitcast (i8** getelementptr inbounds (i8*, i8** @_ZTVN10__cxxabiv121__vmi_class_type_infoE, i64 2) to i8*), i8* getelementptr inbounds ([9 x i8], [9 x i8]* @_ZTS7Derived, i32 0, i32 0), i32 2, i32 2, i8* bitcast ({ i8*, i8*, i32, i32, i8*, i64 }* @_ZTI5BaseB to i8*), i64 2, i8* bitcast ({ i8*, i8*, i32, i32, i8*, i64 }* @_ZTI5BaseC to i8*), i64 4098 }, comdat, align 8

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local i32 @_Z6exeSumv() #0 {
  %1 = alloca %class.Derived, align 8
  call void @_ZN7DerivedC1Ev(%class.Derived* nonnull align 8 dereferenceable(32) %1) #3
  %2 = getelementptr inbounds %class.Derived, %class.Derived* %1, i32 0, i32 3
  store i32 10, i32* %2, align 4
  %3 = call i32 @_ZN7Derived6sumDerEv(%class.Derived* nonnull align 8 dereferenceable(32) %1)
  ret i32 %3
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN7DerivedC1Ev(%class.Derived* nonnull align 8 dereferenceable(32) %0) unnamed_addr #1 comdat align 2 {
  %2 = alloca %class.Derived*, align 8
  store %class.Derived* %0, %class.Derived** %2, align 8
  %3 = load %class.Derived*, %class.Derived** %2, align 8
  %4 = bitcast %class.Derived* %3 to i8*
  %5 = getelementptr inbounds i8, i8* %4, i64 32
  %6 = bitcast i8* %5 to %class.BaseA*
  %7 = bitcast %class.Derived* %3 to %class.BaseB*
  call void @_ZN5BaseBC2Ev(%class.BaseB* nonnull align 8 dereferenceable(12) %7, i8** getelementptr inbounds ([4 x i8*], [4 x i8*]* @_ZTT7Derived, i64 0, i64 1)) #3
  %8 = bitcast %class.Derived* %3 to i8*
  %9 = getelementptr inbounds i8, i8* %8, i64 16
  %10 = bitcast i8* %9 to %class.BaseC*
  call void @_ZN5BaseCC2Ev(%class.BaseC* nonnull align 8 dereferenceable(12) %10, i8** getelementptr inbounds ([4 x i8*], [4 x i8*]* @_ZTT7Derived, i64 0, i64 2)) #3
  %11 = bitcast %class.Derived* %3 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ({ [3 x i8*], [3 x i8*] }, { [3 x i8*], [3 x i8*] }* @_ZTV7Derived, i32 0, inrange i32 0, i32 3) to i32 (...)**), i32 (...)*** %11, align 8
  %12 = bitcast %class.Derived* %3 to i8*
  %13 = getelementptr inbounds i8, i8* %12, i64 16
  %14 = bitcast i8* %13 to i32 (...)***
  store i32 (...)** bitcast (i8** getelementptr inbounds ({ [3 x i8*], [3 x i8*] }, { [3 x i8*], [3 x i8*] }* @_ZTV7Derived, i32 0, inrange i32 1, i32 3) to i32 (...)**), i32 (...)*** %14, align 8
  ret void
}

; Function Attrs: mustprogress noinline optnone uwtable
define linkonce_odr dso_local i32 @_ZN7Derived6sumDerEv(%class.Derived* nonnull align 8 dereferenceable(32) %0) #0 comdat align 2 {
  %2 = alloca %class.Derived*, align 8
  store %class.Derived* %0, %class.Derived** %2, align 8
  %3 = load %class.Derived*, %class.Derived** %2, align 8
  %4 = bitcast %class.Derived* %3 to %class.BaseB*
  %5 = call i32 @_ZN5BaseB4sumBEv(%class.BaseB* nonnull align 8 dereferenceable(12) %4)
  %6 = bitcast %class.Derived* %3 to i8*
  %7 = getelementptr inbounds i8, i8* %6, i64 16
  %8 = bitcast i8* %7 to %class.BaseC*
  %9 = call i32 @_ZN5BaseC4sumCEv(%class.BaseC* nonnull align 8 dereferenceable(12) %8)
  %10 = add nsw i32 %5, %9
  %11 = getelementptr inbounds %class.Derived, %class.Derived* %3, i32 0, i32 3
  %12 = load i32, i32* %11, align 4
  %13 = add nsw i32 %10, %12
  ret i32 %13
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN5BaseBC2Ev(%class.BaseB* nonnull align 8 dereferenceable(12) %0, i8** %1) unnamed_addr #1 comdat align 2 {
  %3 = alloca %class.BaseB*, align 8
  %4 = alloca i8**, align 8
  store %class.BaseB* %0, %class.BaseB** %3, align 8
  store i8** %1, i8*** %4, align 8
  %5 = load %class.BaseB*, %class.BaseB** %3, align 8
  %6 = load i8**, i8*** %4, align 8
  %7 = load i8*, i8** %6, align 8
  %8 = bitcast %class.BaseB* %5 to i32 (...)***
  %9 = bitcast i8* %7 to i32 (...)**
  store i32 (...)** %9, i32 (...)*** %8, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN5BaseCC2Ev(%class.BaseC* nonnull align 8 dereferenceable(12) %0, i8** %1) unnamed_addr #1 comdat align 2 {
  %3 = alloca %class.BaseC*, align 8
  %4 = alloca i8**, align 8
  store %class.BaseC* %0, %class.BaseC** %3, align 8
  store i8** %1, i8*** %4, align 8
  %5 = load %class.BaseC*, %class.BaseC** %3, align 8
  %6 = load i8**, i8*** %4, align 8
  %7 = load i8*, i8** %6, align 8
  %8 = bitcast %class.BaseC* %5 to i32 (...)***
  %9 = bitcast i8* %7 to i32 (...)**
  store i32 (...)** %9, i32 (...)*** %8, align 8
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr dso_local i32 @_ZN5BaseB4sumBEv(%class.BaseB* nonnull align 8 dereferenceable(12) %0) #2 comdat align 2 {
  %2 = alloca %class.BaseB*, align 8
  store %class.BaseB* %0, %class.BaseB** %2, align 8
  %3 = load %class.BaseB*, %class.BaseB** %2, align 8
  %4 = bitcast %class.BaseB* %3 to i8**
  %5 = load i8*, i8** %4, align 8
  %6 = getelementptr i8, i8* %5, i64 -24
  %7 = bitcast i8* %6 to i64*
  %8 = load i64, i64* %7, align 8
  %9 = bitcast %class.BaseB* %3 to i8*
  %10 = getelementptr inbounds i8, i8* %9, i64 %8
  %11 = bitcast i8* %10 to %class.BaseA*
  %12 = getelementptr inbounds %class.BaseA, %class.BaseA* %11, i32 0, i32 0
  %13 = load i32, i32* %12, align 4
  %14 = getelementptr inbounds %class.BaseB, %class.BaseB* %3, i32 0, i32 1
  %15 = load i32, i32* %14, align 8
  %16 = add nsw i32 %13, %15
  ret i32 %16
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr dso_local i32 @_ZN5BaseC4sumCEv(%class.BaseC* nonnull align 8 dereferenceable(12) %0) #2 comdat align 2 {
  %2 = alloca %class.BaseC*, align 8
  store %class.BaseC* %0, %class.BaseC** %2, align 8
  %3 = load %class.BaseC*, %class.BaseC** %2, align 8
  %4 = bitcast %class.BaseC* %3 to i8**
  %5 = load i8*, i8** %4, align 8
  %6 = getelementptr i8, i8* %5, i64 -24
  %7 = bitcast i8* %6 to i64*
  %8 = load i64, i64* %7, align 8
  %9 = bitcast %class.BaseC* %3 to i8*
  %10 = getelementptr inbounds i8, i8* %9, i64 %8
  %11 = bitcast i8* %10 to %class.BaseA*
  %12 = getelementptr inbounds %class.BaseA, %class.BaseA* %11, i32 0, i32 0
  %13 = load i32, i32* %12, align 4
  %14 = getelementptr inbounds %class.BaseC, %class.BaseC* %3, i32 0, i32 1
  %15 = load i32, i32* %14, align 8
  %16 = add nsw i32 %13, %15
  ret i32 %16
}

attributes #0 = { mustprogress noinline optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { mustprogress noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"Ubuntu clang version 13.0.1-++20211217022950+e68f640deecc-1~exp1~20211217023024.30"}
