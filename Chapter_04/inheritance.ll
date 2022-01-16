; ModuleID = 'inheritance.cpp'
source_filename = "inheritance.cpp"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%class.Square = type <{ %class.Box, i32, [4 x i8] }>
%class.Box = type { double }

$_ZN6Square4SetBEi = comdat any

$_ZN3Box4SetAEd = comdat any

; Function Attrs: mustprogress noinline norecurse optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca %class.Square, align 8
  store i32 0, i32* %1, align 4
  call void @_ZN6Square4SetBEi(%class.Square* nonnull align 8 dereferenceable(12) %2, i32 3)
  ret i32 0
}

; Function Attrs: mustprogress noinline optnone uwtable
define linkonce_odr dso_local void @_ZN6Square4SetBEi(%class.Square* nonnull align 8 dereferenceable(12) %0, i32 %1) #1 comdat align 2 {
  %3 = alloca %class.Square*, align 8
  %4 = alloca i32, align 4
  store %class.Square* %0, %class.Square** %3, align 8
  store i32 %1, i32* %4, align 4
  %5 = load %class.Square*, %class.Square** %3, align 8
  %6 = bitcast %class.Square* %5 to %class.Box*
  %7 = load i32, i32* %4, align 4
  %8 = sitofp i32 %7 to double
  call void @_ZN3Box4SetAEd(%class.Box* nonnull align 8 dereferenceable(8) %6, double %8)
  %9 = load i32, i32* %4, align 4
  %10 = getelementptr inbounds %class.Square, %class.Square* %5, i32 0, i32 1
  store i32 %9, i32* %10, align 8
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

attributes #0 = { mustprogress noinline norecurse optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { mustprogress noinline optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { mustprogress noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"Ubuntu clang version 13.0.1-++20211217022950+e68f640deecc-1~exp1~20211217023024.30"}
