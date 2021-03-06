module fckit_array_module
use, intrinsic :: iso_c_binding, only: c_int, c_long, c_float, c_double
implicit none
private

!========================================================================
! Public interface

public :: array_view1d
public :: array_stride
public :: array_strides

!========================================================================

private :: c_int, c_long, c_float, c_double

integer(c_int), target :: zero_length_array_int32(0)
integer(c_long),target :: zero_length_array_int64(0)
real(c_float),  target :: zero_length_array_real32(0)
real(c_double), target :: zero_length_array_real64(0)

INTERFACE array_view1d
  module procedure array_view1d_int32_r0
  module procedure array_view1d_int32_r1
  module procedure array_view1d_int32_r2
  module procedure array_view1d_int32_r3
  module procedure array_view1d_int32_r4
  module procedure array_view1d_int64_r0
  module procedure array_view1d_int64_r1
  module procedure array_view1d_int64_r2
  module procedure array_view1d_int64_r3
  module procedure array_view1d_int64_r4
  module procedure array_view1d_real32_r0
  module procedure array_view1d_real32_r1
  module procedure array_view1d_real32_r2
  module procedure array_view1d_real32_r3
  module procedure array_view1d_real32_r4
  module procedure array_view1d_real64_r0
  module procedure array_view1d_real64_r1
  module procedure array_view1d_real64_r2
  module procedure array_view1d_real64_r3
  module procedure array_view1d_real64_r4
end interface

INTERFACE array_stride
  module procedure array_stride_int32_r1_dim
  module procedure array_stride_int32_r2_dim
  module procedure array_stride_int32_r3_dim
  module procedure array_stride_int32_r4_dim
  module procedure array_stride_int64_r1_dim
  module procedure array_stride_int64_r2_dim
  module procedure array_stride_int64_r3_dim
  module procedure array_stride_int64_r4_dim
  module procedure array_stride_real32_r1_dim
  module procedure array_stride_real32_r2_dim
  module procedure array_stride_real32_r3_dim
  module procedure array_stride_real32_r4_dim
  module procedure array_stride_real64_r1_dim
  module procedure array_stride_real64_r2_dim
  module procedure array_stride_real64_r3_dim
  module procedure array_stride_real64_r4_dim
end interface

INTERFACE array_strides
  module procedure array_stride_int32_r1
  module procedure array_stride_int32_r2
  module procedure array_stride_int32_r3
  module procedure array_stride_int32_r4
  module procedure array_stride_int64_r1
  module procedure array_stride_int64_r2
  module procedure array_stride_int64_r3
  module procedure array_stride_int64_r4
  module procedure array_stride_real32_r1
  module procedure array_stride_real32_r2
  module procedure array_stride_real32_r3
  module procedure array_stride_real32_r4
  module procedure array_stride_real64_r1
  module procedure array_stride_real64_r2
  module procedure array_stride_real64_r3
  module procedure array_stride_real64_r4
end interface

! =============================================================================
CONTAINS
! =============================================================================

function c_loc_int32(x)
  use, intrinsic :: iso_c_binding
  integer(c_int), target :: x
  type(c_ptr) :: c_loc_int32
  c_loc_int32 = c_loc(x)
end function

! =============================================================================

function c_loc_int64(x)
  use, intrinsic :: iso_c_binding
  integer(c_long), target :: x
  type(c_ptr) :: c_loc_int64
  c_loc_int64 = c_loc(x)
end function

! =============================================================================

function c_loc_real32(x)
  use, intrinsic :: iso_c_binding
  real(c_float), target :: x
  type(c_ptr) :: c_loc_real32
  c_loc_real32 = c_loc(x)
end function

! =============================================================================

function c_loc_real64(x)
  use, intrinsic :: iso_c_binding
  real(c_double), target :: x
  type(c_ptr) :: c_loc_real64
  c_loc_real64 = c_loc(x)
end function

! =============================================================================
! view interface
! =============================================================================

function array_view1d_int32_r0(scalar) result( view )
  use, intrinsic :: iso_c_binding
  integer(c_int), intent(in), target :: scalar
  type(c_ptr) :: array_c_ptr
  integer(c_int), pointer :: view(:)
  array_c_ptr = c_loc_int32(scalar)
  call c_f_pointer ( array_c_ptr , view , (/1/) )
end function

! =============================================================================

function array_view1d_int32_r1(array) result( view )
  use, intrinsic :: iso_c_binding
  integer, intent(in), target :: array(:)
  type(c_ptr) :: array_c_ptr
  integer(c_int), pointer :: view(:)
  if( size(array) > 0 ) then
    array_c_ptr = c_loc_int32(array(1))
    call c_f_pointer ( array_c_ptr , view , (/size(array)/) )
  else
    view => zero_length_array_int32
  endif
end function

! =============================================================================

function array_view1d_int32_r2(array) result( view )
  use, intrinsic :: iso_c_binding
  integer, intent(in), target :: array(:,:)
  type(c_ptr) :: array_c_ptr
  integer, pointer :: view(:)
  if( size(array) > 0 ) then
    array_c_ptr = c_loc_int32(array(1,1))
    call c_f_pointer ( array_c_ptr , view , (/size(array)/) )
  else
    view => zero_length_array_int32
  endif
end function

! =============================================================================

function array_view1d_int32_r3(array) result( view )
  use, intrinsic :: iso_c_binding
  integer, intent(in), target :: array(:,:,:)
  type(c_ptr) :: array_c_ptr
  integer, pointer :: view(:)
  if( size(array) > 0 ) then
    array_c_ptr = c_loc_int32(array(1,1,1))
    call c_f_pointer ( array_c_ptr , view , (/size(array)/) )
  else
    view => zero_length_array_int32
  endif
end function

! =============================================================================

function array_view1d_int32_r4(array) result( view )
  use, intrinsic :: iso_c_binding
  integer(c_int), intent(in), target :: array(:,:,:,:)
  type(c_ptr) :: array_c_ptr
  integer(c_int), pointer :: view(:)
  if( size(array) > 0 ) then
    array_c_ptr = c_loc_int32(array(1,1,1,1))
    call c_f_pointer ( array_c_ptr , view , (/size(array)/) )
  else
    view => zero_length_array_int32
  endif
end function

! =============================================================================

function array_view1d_int64_r0(scalar) result( view )
  use, intrinsic :: iso_c_binding
  integer(c_long), intent(in), target :: scalar
  type(c_ptr) :: array_c_ptr
  integer(c_long), pointer :: view(:)
  array_c_ptr = c_loc_int64(scalar)
  call c_f_pointer ( array_c_ptr , view , (/1/) )
end function

! =============================================================================

function array_view1d_int64_r1(array) result( view )
  use, intrinsic :: iso_c_binding
  integer(c_long), intent(in), target :: array(:)
  type(c_ptr) :: array_c_ptr
  integer(c_long), pointer :: view(:)
  if( size(array) > 0 ) then
    array_c_ptr = c_loc_int64(array(1))
    call c_f_pointer ( array_c_ptr , view , (/size(array)/) )
  else
    view => zero_length_array_int64
  endif
end function

! =============================================================================

function array_view1d_int64_r2(array) result( view )
  use, intrinsic :: iso_c_binding
  integer(c_long), intent(in), target :: array(:,:)
  type(c_ptr) :: array_c_ptr
  integer(c_long), pointer :: view(:)
  if( size(array) > 0 ) then
    array_c_ptr = c_loc_int64(array(1,1))
    call c_f_pointer ( array_c_ptr , view , (/size(array)/) )
  else
    view => zero_length_array_int64
  endif
end function

! =============================================================================

function array_view1d_int64_r3(array) result( view )
  use, intrinsic :: iso_c_binding
  integer(c_long), intent(in), target :: array(:,:,:)
  type(c_ptr) :: array_c_ptr
  integer(c_long), pointer :: view(:)
  if( size(array) > 0 ) then
    array_c_ptr = c_loc_int64(array(1,1,1))
    call c_f_pointer ( array_c_ptr , view , (/size(array)/) )
  else
    view => zero_length_array_int64
  endif
end function

! =============================================================================

function array_view1d_int64_r4(array) result( view )
  use, intrinsic :: iso_c_binding
  integer(c_long), intent(in), target :: array(:,:,:,:)
  type(c_ptr) :: array_c_ptr
  integer(c_long), pointer :: view(:)
  if( size(array) > 0 ) then
    array_c_ptr = c_loc_int64(array(1,1,1,1))
    call c_f_pointer ( array_c_ptr , view , (/size(array)/) )
  else
    view => zero_length_array_int64
  endif
end function

! =============================================================================

function array_view1d_real32_r0(scalar) result( view )
  use, intrinsic :: iso_c_binding
  real(c_float), intent(in), target :: scalar
  type(c_ptr) :: array_c_ptr
  real(c_float), pointer :: view(:)
  array_c_ptr = c_loc_real32(scalar)
  call c_f_pointer ( array_c_ptr , view , (/1/) )
end function

! =============================================================================

function array_view1d_real32_r1(array) result( view )
  use, intrinsic :: iso_c_binding
  real(c_float), intent(in), target :: array(:)
  type(c_ptr) :: array_c_ptr
  real(c_float), pointer :: view(:)
  if( size(array) > 0 ) then
    array_c_ptr = c_loc_real32(array(1))
    call c_f_pointer ( array_c_ptr , view , (/size(array)/) )
  else
    view => zero_length_array_real32
  endif
end function

! =============================================================================

function array_view1d_real32_r2(array) result( view )
  use, intrinsic :: iso_c_binding
  real(c_float), intent(in), target :: array(:,:)
  type(c_ptr) :: array_c_ptr
  real(c_float), pointer :: view(:)
  if( size(array) > 0 ) then
    array_c_ptr = c_loc_real32(array(1,1))
    call c_f_pointer ( array_c_ptr , view , (/size(array)/) )
  else
    view => zero_length_array_real32
  endif
end function

! =============================================================================

function array_view1d_real32_r3(array) result( view )
  use, intrinsic :: iso_c_binding
  real(c_float), intent(in), target :: array(:,:,:)
  type(c_ptr) :: array_c_ptr
  real(c_float), pointer :: view(:)
  if( size(array) > 0 ) then
    array_c_ptr = c_loc_real32(array(1,1,1))
    call c_f_pointer ( array_c_ptr , view , (/size(array)/) )
  else
    view => zero_length_array_real32
  endif
end function

! =============================================================================

function array_view1d_real32_r4(array) result( view )
  use, intrinsic :: iso_c_binding
  real(c_float), intent(in), target :: array(:,:,:,:)
  type(c_ptr) :: array_c_ptr
  real(c_float), pointer :: view(:)
  if( size(array) > 0 ) then
    array_c_ptr = c_loc_real32(array(1,1,1,1))
    call c_f_pointer ( array_c_ptr , view , (/size(array)/) )
  else
    view => zero_length_array_real32
  endif
end function

! =============================================================================

function array_view1d_real64_r0(scalar) result( view )
  use, intrinsic :: iso_c_binding
  real(c_double), intent(in), target :: scalar
  type(c_ptr) :: array_c_ptr
  real(c_double), pointer :: view(:)
  array_c_ptr = c_loc_real64(scalar)
  call c_f_pointer ( array_c_ptr , view , (/1/) )
end function

! =============================================================================

function array_view1d_real64_r1(array) result( view )
  use, intrinsic :: iso_c_binding
  real(c_double), intent(in), target :: array(:)
  type(c_ptr) :: array_c_ptr
  real(c_double), pointer :: view(:)
  if( size(array) > 0 ) then
    array_c_ptr = c_loc_real64(array(1))
    call c_f_pointer ( array_c_ptr , view , (/size(array)/) )
  else
    view => zero_length_array_real64
  endif
end function

! =============================================================================

function array_view1d_real64_r2(array) result( view )
  use, intrinsic :: iso_c_binding
  real(c_double), intent(in), target :: array(:,:)
  type(c_ptr) :: array_c_ptr
  real(c_double), pointer :: view(:)
  if( size(array) > 0 ) then
    array_c_ptr = c_loc_real64(array(1,1))
    call c_f_pointer ( array_c_ptr , view , (/size(array)/) )
  else
    view => zero_length_array_real64
  endif
end function

! =============================================================================

function array_view1d_real64_r3(array) result( view )
  use, intrinsic :: iso_c_binding
  real(c_double), intent(in), target :: array(:,:,:)
  type(c_ptr) :: array_c_ptr
  real(c_double), pointer :: view(:)
  if( size(array) > 0 ) then
    array_c_ptr = c_loc_real64(array(1,1,1))
    call c_f_pointer ( array_c_ptr , view , (/size(array)/) )
  else
    view => zero_length_array_real64
  endif
end function

! =============================================================================

function array_view1d_real64_r4(array) result( view )
  use, intrinsic :: iso_c_binding
  real(c_double), intent(in), target :: array(:,:,:,:)
  type(c_ptr) :: array_c_ptr
  real(c_double), pointer :: view(:)
  if( size(array) > 0 ) then
    array_c_ptr = c_loc_real64(array(1,1,1,1))
    call c_f_pointer ( array_c_ptr , view , (/size(array)/) )
  else
    view => zero_length_array_real64
  endif
end function

! =============================================================================
! stride interface

function array_stride_int32_r1_dim(arr,dim) result( stride )
  use, intrinsic :: iso_c_binding
  integer(c_int) :: arr(:)
  integer(c_int) :: dim
  integer(c_int) :: stride
  stride = 1
  if (dim == 1 .AND. ubound(arr,1) > 1) stride = (loc(arr(2))-loc(arr(1)))/4
end function

! =============================================================================

function array_stride_int32_r2_dim(arr,dim) result( stride )
  use, intrinsic :: iso_c_binding
  integer(c_int) :: arr(:,:)
  integer(c_int) :: dim
  integer(c_int) :: stride
  stride = 1
  if( size(arr) > 0 ) then
    if (dim == 1 .AND. ubound(arr,1) > 1) stride = (loc(arr(2,1))-loc(arr(1,1)))/4
    if (dim == 2 .AND. ubound(arr,1) > 1) stride = (loc(arr(1,2))-loc(arr(1,1)))/4
  else
    stride = 0
  endif
end function

! =============================================================================

function array_stride_int32_r3_dim(arr,dim) result( stride )
  use, intrinsic :: iso_c_binding
  integer(c_int) :: arr(:,:,:)
  integer(c_int) :: dim
  integer(c_int) :: stride
  stride = 1
  if( size(arr) > 0 ) then
    if (dim == 1 .AND. ubound(arr,1) > 1) stride = (loc(arr(2,1,1))-loc(arr(1,1,1)))/4
    if (dim == 2 .AND. ubound(arr,2) > 1) stride = (loc(arr(1,2,1))-loc(arr(1,1,1)))/4
    if (dim == 3 .AND. ubound(arr,3) > 1) stride = (loc(arr(1,1,2))-loc(arr(1,1,1)))/4
  else
    stride = 0
  endif
end function

! =============================================================================

function array_stride_int32_r4_dim(arr,dim) result( stride )
  use, intrinsic :: iso_c_binding
  integer(c_int) :: arr(:,:,:,:)
  integer(c_int) :: dim
  integer(c_int) :: stride
  stride = 1
  if( size(arr) > 0 ) then
    if (dim == 1 .AND. ubound(arr,1) > 1) stride = (loc(arr(2,1,1,1))-loc(arr(1,1,1,1)))/4
    if (dim == 2 .AND. ubound(arr,2) > 1) stride = (loc(arr(1,2,1,1))-loc(arr(1,1,1,1)))/4
    if (dim == 3 .AND. ubound(arr,3) > 1) stride = (loc(arr(1,1,2,1))-loc(arr(1,1,1,1)))/4
    if (dim == 4 .AND. ubound(arr,4) > 1) stride = (loc(arr(1,1,1,2))-loc(arr(1,1,1,1)))/4
  else
    stride = 0
  endif
end function

! =============================================================================

function array_stride_int64_r1_dim(arr,dim) result( stride )
  use, intrinsic :: iso_c_binding
  integer(c_long) :: arr(:)
  integer(c_int) :: dim
  integer(c_int) :: stride
  stride = 1
  if (dim == 1 .AND. ubound(arr,1) > 1) stride = (loc(arr(2))-loc(arr(1)))/4
end function

! =============================================================================

function array_stride_int64_r2_dim(arr,dim) result( stride )
  use, intrinsic :: iso_c_binding
  integer(c_long) :: arr(:,:)
  integer(c_int) :: dim
  integer(c_int) :: stride
  stride = 1
  if( size(arr) > 0 ) then
    if (dim == 1 .AND. ubound(arr,1) > 1) stride = (loc(arr(2,1))-loc(arr(1,1)))/4
    if (dim == 2 .AND. ubound(arr,1) > 1) stride = (loc(arr(1,2))-loc(arr(1,1)))/4
  else
    stride = 0
  endif
end function

! =============================================================================

function array_stride_int64_r3_dim(arr,dim) result( stride )
  use, intrinsic :: iso_c_binding
  integer(c_long) :: arr(:,:,:)
  integer(c_int) :: dim
  integer(c_int) :: stride
  stride = 1
  if( size(arr) > 0 ) then
    if (dim == 1 .AND. ubound(arr,1) > 1) stride = (loc(arr(2,1,1))-loc(arr(1,1,1)))/4
    if (dim == 2 .AND. ubound(arr,2) > 1) stride = (loc(arr(1,2,1))-loc(arr(1,1,1)))/4
    if (dim == 3 .AND. ubound(arr,3) > 1) stride = (loc(arr(1,1,2))-loc(arr(1,1,1)))/4
  else
    stride = 0
  endif
end function

! =============================================================================

function array_stride_int64_r4_dim(arr,dim) result( stride )
  use, intrinsic :: iso_c_binding
  integer(c_long) :: arr(:,:,:,:)
  integer(c_int) :: dim
  integer(c_int) :: stride
  stride = 1
  if( size(arr) > 0 ) then
    if (dim == 1 .AND. ubound(arr,1) > 1) stride = (loc(arr(2,1,1,1))-loc(arr(1,1,1,1)))/4
    if (dim == 2 .AND. ubound(arr,2) > 1) stride = (loc(arr(1,2,1,1))-loc(arr(1,1,1,1)))/4
    if (dim == 3 .AND. ubound(arr,3) > 1) stride = (loc(arr(1,1,2,1))-loc(arr(1,1,1,1)))/4
    if (dim == 4 .AND. ubound(arr,4) > 1) stride = (loc(arr(1,1,1,2))-loc(arr(1,1,1,1)))/4
  else
    stride = 0
  endif
end function

! =============================================================================

function array_stride_real32_r1_dim(arr,dim) result( stride )
  use, intrinsic :: iso_c_binding
  real(c_float) :: arr(:)
  integer(c_int) :: dim
  integer(c_int) :: stride
  stride = 1
  if( size(arr) > 0 ) then
    if (dim == 1 .AND. ubound(arr,1) > 1) stride = (loc(arr(2))-loc(arr(1)))/4
  else
    stride = 0
  endif
end function

! =============================================================================

function array_stride_real32_r2_dim(arr,dim) result( stride )
  use, intrinsic :: iso_c_binding
  real(c_float) :: arr(:,:)
  integer(c_int) :: dim
  integer(c_int) :: stride
  stride = 1
  if( size(arr) > 0 ) then
    if (dim == 1 .AND. ubound(arr,1) > 1) stride = (loc(arr(2,1))-loc(arr(1,1)))/4
    if (dim == 2 .AND. ubound(arr,2) > 1) stride = (loc(arr(1,2))-loc(arr(1,1)))/4
  else
    stride = 0
  endif
end function

! =============================================================================

function array_stride_real32_r3_dim(arr,dim) result( stride )
  use, intrinsic :: iso_c_binding
  real(c_float) :: arr(:,:,:)
  integer(c_int) :: dim
  integer(c_int) :: stride
  stride = 1
  if( size(arr) > 0 ) then
    if (dim == 1 .AND. ubound(arr,1) > 1) stride = (loc(arr(2,1,1))-loc(arr(1,1,1)))/4
    if (dim == 2 .AND. ubound(arr,2) > 1) stride = (loc(arr(1,2,1))-loc(arr(1,1,1)))/4
    if (dim == 3 .AND. ubound(arr,3) > 1) stride = (loc(arr(1,1,2))-loc(arr(1,1,1)))/4
  else
    stride = 0
  endif
end function

! =============================================================================

function array_stride_real32_r4_dim(arr,dim) result( stride )
  use, intrinsic :: iso_c_binding
  real(c_float) :: arr(:,:,:,:)
  integer(c_int) :: dim
  integer(c_int) :: stride
  stride = 1
  if( size(arr) > 0 ) then
    if (dim == 1 .AND. ubound(arr,1) > 1) stride = (loc(arr(2,1,1,1))-loc(arr(1,1,1,1)))/4
    if (dim == 2 .AND. ubound(arr,2) > 1) stride = (loc(arr(1,2,1,1))-loc(arr(1,1,1,1)))/4
    if (dim == 3 .AND. ubound(arr,3) > 1) stride = (loc(arr(1,1,2,1))-loc(arr(1,1,1,1)))/4
    if (dim == 4 .AND. ubound(arr,4) > 1) stride = (loc(arr(1,1,1,2))-loc(arr(1,1,1,1)))/4
  else
    stride = 0
  endif
end function

! =============================================================================

function array_stride_real64_r1_dim(arr,dim) result( stride )
  use, intrinsic :: iso_c_binding
  real(c_double) :: arr(:)
  integer(c_int) :: dim
  integer(c_int) :: stride
  stride = 1
  if( size(arr) > 0 ) then
    if (dim == 1 .AND. ubound(arr,1) > 1) stride = (loc(arr(2))-loc(arr(1)))/8
  else
    stride = 0
  endif
end function

! =============================================================================

function array_stride_real64_r2_dim(arr,dim) result( stride )
  use, intrinsic :: iso_c_binding
  real(c_double) :: arr(:,:)
  integer(c_int) :: dim
  integer(c_int) :: stride
  stride = 1
  if( size(arr)>0 ) then
    if (dim == 1 .AND. ubound(arr,1) > 1) stride = (loc(arr(2,1))-loc(arr(1,1)))/8
    if (dim == 2 .AND. ubound(arr,2) > 1) stride = (loc(arr(1,2))-loc(arr(1,1)))/8
  else
    stride = 0
  endif
end function

! =============================================================================

function array_stride_real64_r3_dim(arr,dim) result( stride )
  use, intrinsic :: iso_c_binding
  real(c_double) :: arr(:,:,:)
  integer(c_int) :: dim
  integer(c_int) :: stride
  stride = 1
  if( size(arr)>0 ) then
    if (dim == 1 .AND. ubound(arr,1) > 1) stride = (loc(arr(2,1,1))-loc(arr(1,1,1)))/8
    if (dim == 2 .AND. ubound(arr,2) > 1) stride = (loc(arr(1,2,1))-loc(arr(1,1,1)))/8
    if (dim == 3 .AND. ubound(arr,3) > 1) stride = (loc(arr(1,1,2))-loc(arr(1,1,1)))/8
  else
    stride = 0
  endif
end function

! =============================================================================

function array_stride_real64_r4_dim(arr,dim) result( stride )
  use, intrinsic :: iso_c_binding
  real(c_double) :: arr(:,:,:,:)
  integer(c_int) :: dim
  integer(c_int) :: stride
  stride = 1
  if( size(arr)>0 ) then
    if (dim == 1 .AND. ubound(arr,1) > 1) stride = (loc(arr(2,1,1,1))-loc(arr(1,1,1,1)))/8
    if (dim == 2) stride = (loc(arr(1,2,1,1))-loc(arr(1,1,1,1)))/8
    if (dim == 3) stride = (loc(arr(1,1,2,1))-loc(arr(1,1,1,1)))/8
    if (dim == 4) stride = (loc(arr(1,1,1,2))-loc(arr(1,1,1,1)))/8
  else
    stride = 0
  endif
end function

! =============================================================================
! stride interface

function array_stride_int32_r1(arr) result( stride_ )
  use, intrinsic :: iso_c_binding
  integer(c_int) :: arr(:)
  integer(c_int) :: stride_(1)
  stride_(1) = array_stride_int32_r1_dim(arr,1)
end function

! =============================================================================

function array_stride_int32_r2(arr) result( stride_ )
  use, intrinsic :: iso_c_binding
  integer(c_int) :: arr(:,:)
  integer(c_int) :: stride_(2)
  stride_(1) = array_stride_int32_r2_dim(arr,1)
  stride_(2) = array_stride_int32_r2_dim(arr,2)
end function

! =============================================================================

function array_stride_int32_r3(arr) result( stride_ )
  use, intrinsic :: iso_c_binding
  integer(c_int) :: arr(:,:,:)
  integer(c_int) :: stride_(3)
  stride_(1) = array_stride_int32_r3_dim(arr,1)
  stride_(2) = array_stride_int32_r3_dim(arr,2)
  stride_(3) = array_stride_int32_r3_dim(arr,3)
end function

! =============================================================================

function array_stride_int32_r4(arr) result( stride_ )
  use, intrinsic :: iso_c_binding
  integer(c_int) :: arr(:,:,:,:)
  integer(c_int) :: stride_(4)
  stride_(1) = array_stride_int32_r4_dim(arr,1)
  stride_(2) = array_stride_int32_r4_dim(arr,2)
  stride_(3) = array_stride_int32_r4_dim(arr,3)
  stride_(4) = array_stride_int32_r4_dim(arr,4)
end function

! =============================================================================

function array_stride_int64_r1(arr) result( stride_ )
  use, intrinsic :: iso_c_binding
  integer(c_long) :: arr(:)
  integer(c_int) :: stride_(1)
  stride_(1) = array_stride_int64_r1_dim(arr,1)
end function

! =============================================================================

function array_stride_int64_r2(arr) result( stride_ )
  use, intrinsic :: iso_c_binding
  integer(c_long) :: arr(:,:)
  integer(c_int) :: stride_(2)
  stride_(1) = array_stride_int64_r2_dim(arr,1)
  stride_(2) = array_stride_int64_r2_dim(arr,2)
end function

! =============================================================================

function array_stride_int64_r3(arr) result( stride_ )
  use, intrinsic :: iso_c_binding
  integer(c_long) :: arr(:,:,:)
  integer(c_int) :: stride_(3)
  stride_(1) = array_stride_int64_r3_dim(arr,1)
  stride_(2) = array_stride_int64_r3_dim(arr,2)
  stride_(3) = array_stride_int64_r3_dim(arr,3)
end function

! =============================================================================

function array_stride_int64_r4(arr) result( stride_ )
  use, intrinsic :: iso_c_binding
  integer(c_long) :: arr(:,:,:,:)
  integer(c_int) :: stride_(4)
  stride_(1) = array_stride_int64_r4_dim(arr,1)
  stride_(2) = array_stride_int64_r4_dim(arr,2)
  stride_(3) = array_stride_int64_r4_dim(arr,3)
  stride_(4) = array_stride_int64_r4_dim(arr,4)
end function

! =============================================================================

function array_stride_real32_r1(arr) result( stride_ )
  use, intrinsic :: iso_c_binding
  real(c_float) :: arr(:)
  integer(c_int) :: stride_(1)
  stride_(1) = array_stride_real32_r1_dim(arr,1)
end function

! =============================================================================

function array_stride_real32_r2(arr) result( stride_ )
  use, intrinsic :: iso_c_binding
  real(c_float) :: arr(:,:)
  integer(c_int) :: stride_(2)
  stride_(1) = array_stride_real32_r2_dim(arr,1)
  stride_(2) = array_stride_real32_r2_dim(arr,2)
end function

! =============================================================================

function array_stride_real32_r3(arr) result( stride_ )
  use, intrinsic :: iso_c_binding
  real(c_float) :: arr(:,:,:)
  integer(c_int) :: stride_(3)
  stride_(1) = array_stride_real32_r3_dim(arr,1)
  stride_(2) = array_stride_real32_r3_dim(arr,2)
  stride_(3) = array_stride_real32_r3_dim(arr,3)
end function

! =============================================================================

function array_stride_real32_r4(arr) result( stride_ )
  use, intrinsic :: iso_c_binding
  real(c_float) :: arr(:,:,:,:)
  integer(c_int) :: stride_(4)
  stride_(1) = array_stride_real32_r4_dim(arr,1)
  stride_(2) = array_stride_real32_r4_dim(arr,2)
  stride_(3) = array_stride_real32_r4_dim(arr,3)
  stride_(4) = array_stride_real32_r4_dim(arr,4)
end function

! =============================================================================

function array_stride_real64_r1(arr) result( stride_ )
  use, intrinsic :: iso_c_binding
  real(c_double) :: arr(:)
  integer(c_int) :: stride_(1)
  stride_(1) = array_stride_real64_r1_dim(arr,1)
end function

! =============================================================================

function array_stride_real64_r2(arr) result( stride_ )
  use, intrinsic :: iso_c_binding
  real(c_double) :: arr(:,:)
  integer(c_int) :: stride_(2)
  stride_(1) = array_stride_real64_r2_dim(arr,1)
  stride_(2) = array_stride_real64_r2_dim(arr,2)
end function

! =============================================================================

function array_stride_real64_r3(arr) result( stride_ )
  use, intrinsic :: iso_c_binding
  real(c_double) :: arr(:,:,:)
  integer(c_int) :: stride_(3)
  stride_(1) = array_stride_real64_r3_dim(arr,1)
  stride_(2) = array_stride_real64_r3_dim(arr,2)
  stride_(3) = array_stride_real64_r3_dim(arr,3)
end function

! =============================================================================

function array_stride_real64_r4(arr) result( stride_ )
  use, intrinsic :: iso_c_binding
  real(c_double) :: arr(:,:,:,:)
  integer(c_int) :: stride_(4)
  stride_(1) = array_stride_real64_r4_dim(arr,1)
  stride_(2) = array_stride_real64_r4_dim(arr,2)
  stride_(3) = array_stride_real64_r4_dim(arr,3)
  stride_(4) = array_stride_real64_r4_dim(arr,4)
end function

! =============================================================================

end module
