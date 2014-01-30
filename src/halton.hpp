/*
	John Halton,
	On the efficiency of certain quasi-random sequences of points in evaluating multi-dimensional integrals,
	Numerische Mathematik,
	Volume 2, 1960, pages 84-90.
	John Halton, GB Smith,
	Algorithm 247: Radical-Inverse Quasi-Random Point Sequence,
	Communications of the ACM,
	Volume 7, 1964, pages 701-702.
	Ladislav Kocis, William Whiten,
	Computational Investigations of Low-Discrepancy Sequences,
	ACM Transactions on Mathematical Software,
	Volume 23, Number 2, 1997, pages 266-294.
*/


#pragma once

double arc_cosine ( double c );
double atan4 ( double y, double x );
char digit_to_ch ( int i );
int get_seed ( void );
bool halham_leap_check ( int dim_num, int leap[] );
bool halham_n_check ( int n );
bool halham_dim_num_check ( int dim_num );
bool halham_seed_check ( int dim_num, int seed[] );
bool halham_step_check ( int step );
void halham_write ( int dim_num, int n, int step, int seed[], int leap[], int base[], 
				   double r[], char *file_out_name );
void halton ( double r[] );
bool halton_base_check ( int dim_num, int base[] );
int *halton_base_get ( void );
void halton_base_set ( int base[] );
int *halton_leap_get ( void );
void halton_leap_set ( int leap[] );
int halton_dim_num_get ( void );
void halton_dim_num_set ( int dim_num );
int *halton_seed_get ( void );
void halton_seed_set ( int seed[] );
void halton_sequence ( int n, double r[] );
int halton_step_get ( void );
void halton_step_set ( int step );
int i4_log_10 ( int i );
int i4_min ( int i1, int i2 );
void i4_to_halton ( int dim_num, int step, int seed[], int leap[], int base[], 
				   double r[] );
void i4_to_halton_sequence ( int dim_num, int n, int step, int seed[], int leap[],
							int base[], double r[] );
char *i4_to_s ( int i );
void i4vec_transpose_print ( int n, int a[], char *title );
int prime ( int n );
double r8_epsilon ( void );
double r8vec_dot_product ( int n, double *r1, double *r2 );
double r8vec_norm_l2 ( int n, double a[] );
int s_len_trim ( char *s );
void timestamp ( void );
char *timestring ( void );
void u1_to_sphere_unit_2d ( double u[1], double x[2] );
void u2_to_ball_unit_2d ( double u[2], double x[2] );
void u2_to_sphere_unit_3d ( double u[2], double x[3] );
void u3_to_ball_unit_3d ( double u[3], double x[3] );