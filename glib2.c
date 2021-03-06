/* search functions */

#include <stdlib.h>
#include <math.h>

#include "glib.h"

extern ret *R;
extern ecf **S;

extern int R_n;

ret *ret_srch(ecf *strt, int guess, int year, int mnth) {

	int i, j, s, same_firm, crct_year, crct_mnth;
	ret *crnt;
	
	i = strt->rkey+guess;
	
	for (j = 0, s = -1; j < MLSR; j += (s+1)/2, s *= -1) {
		
		if (i+s*j < R_n && i+s*j >= 0) {
		
			crnt = R+i+s*j;
		
			same_firm = (crnt->firm == strt->firm); /* same firm? */
			crct_year = (crnt->year == year); /* correct year? */
			crct_mnth = (crnt->mnth == mnth); /* correct month? */
				
			if (same_firm && crct_year && crct_mnth)
				return crnt;
		}	
	}

	return NULL;
}

int ecf_srch(ecf *s[], ecf *evnt, ymp *date, mfs *spec) {

	int i, j, f = 0, sgn, diff_firm, same_year, same_mnth, same_rnge, same_qntl, **H = date->hash, F_n = date->n;
	double evnt_fctr2 = (spec->fctr2 == -1) ? 1.: evnt->fctr[spec->fctr2], ctrl_fctr2;
	ecf *ctrl, *F = date->pntr;

	i = evnt->fkey;

	for (j = 0, sgn = -1; f < MNCF+1 && j < F_n; j += (sgn+1)/2, sgn *= -1) {
		
		if (i+sgn*j < F_n && i+sgn*j >= 0) {
		
			ctrl = F+H[i+sgn*j][spec->fctr1];
			
			ctrl_fctr2 = ctrl->fctr[spec->fctr2];
			
			diff_firm = (evnt->firm != ctrl->firm); 
			same_year = (evnt->year == ctrl->year);
			same_mnth = (evnt->mnth == ctrl->mnth);
			same_rnge = (spec->fctr2 == -1) ? 1 : (spec->lower*evnt_fctr2 <= ctrl_fctr2 && ctrl_fctr2 <= spec->upper*evnt_fctr2); 
			same_qntl = (spec->fctr3 == -1) ? 1 : (evnt->rank[spec->fctr3] == ctrl->rank[spec->fctr3]); 
			
			if (diff_firm && same_year && same_mnth && same_rnge) {
			
				s[f++] = ctrl; 
					
				if (f == 1) {
					evnt->twin1[spec->fctr1] = ctrl->fctr[spec->fctr1];
					evnt->twin2[spec->fctr2] = ctrl->fctr[spec->fctr2];
				}
			}
		}
	}
	
	return (f < MNCF) ? 1 : 0;
}