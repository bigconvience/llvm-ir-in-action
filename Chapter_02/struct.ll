; ModuleID = 'struct.c'
source_filename = "struct.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.Point = type { double, double, double }

@__const.main.a = private unnamed_addr constant %struct.Point { double 1.000000e+00, double 3.000000e+00, double 4.000000e+00 }, align 8
@__const.main.b = private unnamed_addr constant %struct.Point { double 2.000000e+00, double 8.000000e+00, double 5.000000e+00 }, align 8

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @add_points(%struct.Point* noalias sret(%struct.Point) align 8 %0, %struct.Point* byval(%struct.Point) align 8 %1, %struct.Point* byval(%struct.Point) align 8 %2) #0 {
  %4 = getelementptr inbounds %struct.Point, %struct.Point* %1, i32 0, i32 0
  %5 = load double, double* %4, align 8
  %6 = getelementptr inbounds %struct.Point, %struct.Point* %2, i32 0, i32 0
  %7 = load double, double* %6, align 8
  %8 = fadd double %5, %7
  %9 = getelementptr inbounds %struct.Point, %struct.Point* %0, i32 0, i32 0
  store double %8, double* %9, align 8
  %10 = getelementptr inbounds %struct.Point, %struct.Point* %1, i32 0, i32 1
  %11 = load double, double* %10, align 8
  %12 = getelementptr inbounds %struct.Point, %struct.Point* %2, i32 0, i32 1
  %13 = load double, double* %12, align 8
  %14 = fadd double %11, %13
  %15 = getelementptr inbounds %struct.Point, %struct.Point* %0, i32 0, i32 1
  store double %14, double* %15, align 8
  %16 = getelementptr inbounds %struct.Point, %struct.Point* %1, i32 0, i32 2
  %17 = load double, double* %16, align 8
  %18 = getelementptr inbounds %struct.Point, %struct.Point* %2, i32 0, i32 2
  %19 = load double, double* %18, align 8
  %20 = fadd double %17, %19
  %21 = getelementptr inbounds %struct.Point, %struct.Point* %0, i32 0, i32 2
  store double %20, double* %21, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca %struct.Point, align 8
  %3 = alloca %struct.Point, align 8
  %4 = alloca %struct.Point, align 8
  store i32 0, i32* %1, align 4
  %5 = bitcast %struct.Point* %2 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %5, i8* align 8 bitcast (%struct.Point* @__const.main.a to i8*), i64 24, i1 false)
  %6 = bitcast %struct.Point* %3 to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* align 8 %6, i8* align 8 bitcast (%struct.Point* @__const.main.b to i8*), i64 24, i1 false)
  call void @add_points(%struct.Point* sret(%struct.Point) align 8 %4, %struct.Point* byval(%struct.Point) align 8 %2, %struct.Point* byval(%struct.Point) align 8 %3)
  ret i32 0
}

; Function Attrs: argmemonly nofree nounwind willreturn
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* noalias nocapture writeonly, i8* noalias nocapture readonly, i64, i1 immarg) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { argmemonly nofree nounwind willreturn }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 1}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"Ubuntu clang version 13.0.1-++20211217022950+e68f640deecc-1~exp1~20211217023024.30"}
