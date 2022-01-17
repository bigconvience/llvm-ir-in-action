; ModuleID = 'inheritance.cpp'
source_filename = "inheritance.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%class.Cube = type <{ %class.Box, %class.Square, i32, [4 x i8] }>
%class.Box = type { double }
%class.Square = type { double }

$_ZN4Cube4SetCEd = comdat any

$_ZN3Box4SetAEd = comdat any

$_ZN6Square4SetBEd = comdat any

; Function Attrs: mustprogress noinline norecurse optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca %class.Cube, align 8
  store i32 0, i32* %1, align 4
  call void @_ZN4Cube4SetCEd(%class.Cube* nonnull align 8 dereferenceable(20) %2, double 3.000000e+00)
  ret i32 0
}

; Function Attrs: mustprogress noinline optnone uwtable
define linkonce_odr dso_local void @_ZN4Cube4SetCEd(%class.Cube* nonnull align 8 dereferenceable(20) %0, double %1) #1 comdat align 2 {
  %3 = alloca %class.Cube*, align 8
  %4 = alloca double, align 8
  store %class.Cube* %0, %class.Cube** %3, align 8
  store double %1, double* %4, align 8
  %5 = load %class.Cube*, %class.Cube** %3, align 8
  %6 = bitcast %class.Cube* %5 to %class.Box*
  %7 = load double, double* %4, align 8
  call void @_ZN3Box4SetAEd(%class.Box* nonnull align 8 dereferenceable(8) %6, double %7)
  %8 = bitcast %class.Cube* %5 to i8*
  %9 = getelementptr inbounds i8, i8* %8, i64 8
  %10 = bitcast i8* %9 to %class.Square*
  %11 = load double, double* %4, align 8
  call void @_ZN6Square4SetBEd(%class.Square* nonnull align 8 dereferenceable(8) %10, double %11)
  %12 = load double, double* %4, align 8
  %13 = fptosi double %12 to i32
  %14 = getelementptr inbounds %class.Cube, %class.Cube* %5, i32 0, i32 2
  store i32 %13, i32* %14, align 8
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN3Box4SetAEd(%class.Box* nonnull align 8 dereferenceable(8) %0, double %1) #2 comdat align 2 {
  %3 = alloca %class.Box*, align 8
  %4 = alloca double, align 8
  store %class.Box* %0, %class.Box** %3, align 8
  store double %1, double* %4, align 8
  %5 = load %class.Box*, %class.Box** %3, align 8
  %6 = load double, double* %4, align 8
  %7 = getelementptr inbounds %class.Box, %class.Box* %5, i32 0, i32 0
  store double %6, double* %7, align 8
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define linkonce_odr dso_local void @_ZN6Square4SetBEd(%class.Square* nonnull align 8 dereferenceable(8) %0, double %1) #2 comdat align 2 {
  %3 = alloca %class.Square*, align 8
  %4 = alloca double, align 8
  store %class.Square* %0, %class.Square** %3, align 8
  store double %1, double* %4, align 8
  %5 = load %class.Square*, %class.Square** %3, align 8
  %6 = load double, double* %4, align 8
  %7 = getelementptr inbounds %class.Square, %class.Square* %5, i32 0, i32 0
  store double %6, double* %7, align 8
  ret void
}

attributes #0 = { mustprogress noinline norecurse optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress noinline optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { mustprogress noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"Ubuntu clang version 13.0.1-++20211217022950+e68f640deecc-1~exp1~20211217023024.30"}
