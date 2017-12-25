%ifndef WIN32
%define codeseg code32
%define dataseg code32
%endif
%ifdef WIN32
%define codeseg text
%define dataseg .data
%endif