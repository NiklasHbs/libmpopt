%module libmpopt_mwis_bregman_log
%{
  #include <mpopt/mwis_bregman_log.h>
%}

%typemap(in) int* {
  $1 = (int*)(PyInt_AsLong($input));
}

%rename ("%(strip:[mpopt_mwis_])s") "";
%include <mpopt/mwis_bregman_log.h>
