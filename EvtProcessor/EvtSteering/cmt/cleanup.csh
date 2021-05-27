# echo "cleanup EvtSteering v0 in /junofs/users/liuyan/OEC/oec-prototype/EvtProcessor"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r1-branch/ExternalLibs/CMT/v1r26
endif
source ${CMTROOT}/mgr/setup.csh
set cmtEvtSteeringtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtEvtSteeringtempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=EvtSteering -version=v0 -path=/junofs/users/liuyan/OEC/oec-prototype/EvtProcessor  $* >${cmtEvtSteeringtempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt cleanup -csh -pack=EvtSteering -version=v0 -path=/junofs/users/liuyan/OEC/oec-prototype/EvtProcessor  $* >${cmtEvtSteeringtempfile}"
  set cmtcleanupstatus=2
  /bin/rm -f ${cmtEvtSteeringtempfile}
  unset cmtEvtSteeringtempfile
  exit $cmtcleanupstatus
endif
set cmtcleanupstatus=0
source ${cmtEvtSteeringtempfile}
if ( $status != 0 ) then
  set cmtcleanupstatus=2
endif
/bin/rm -f ${cmtEvtSteeringtempfile}
unset cmtEvtSteeringtempfile
exit $cmtcleanupstatus

