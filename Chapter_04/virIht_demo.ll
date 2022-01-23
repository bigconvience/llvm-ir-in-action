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

$_ZN7Derived6sumDerEv = comdat any

$_ZN5BaseB4sumBEv = comdat any

$_ZN5BaseC4sumCEv = comdat any

; Function Attrs: mustprogress noinline optnone uwtable
define dso_local i32 @_Z3sumP7Derived(%class.Derived* %0) #0 {
  %2 = alloca %class.Derived*, align 8
  store %class.Derived* %0, %class.Derived** %2, align 8
  %3 = load %class.Derived*, %class.Derived** %2, align 8
  %4 = call i32 @_ZN7Derived6sumDerEv(%class.Derived* nonnull align 8 dereferenceable(32) %3)
  ret i32 %4
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

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr dso_local i32 @_ZN5BaseB4sumBEv(%class.BaseB* nonnull align 8 dereferenceable(12) %0) #1 comdat align 2 {
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
define linkonce_odr dso_local i32 @_ZN5BaseC4sumCEv(%class.BaseC* nonnull align 8 dereferenceable(12) %0) #1 comdat align 2 {
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
attributes #1 = { mustprogress noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"Ubuntu clang version 13.0.1-++20211217022950+e68f640deecc-1~exp1~20211217023024.30"}
