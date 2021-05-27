# echo "setup EvtSteering v0 in /junofs/users/liuyan/OEC/oec-prototype/EvtProcessor"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r1-branch/ExternalLibs/CMT/v1r26; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtEvtSteeringtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtEvtSteeringtempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=EvtSteering -version=v0 -path=/junofs/users/liuyan/OEC/oec-prototype/EvtProcessor  -no_cleanup $* >${cmtEvtSteeringtempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=EvtSteering -version=v0 -path=/junofs/users/liuyan/OEC/oec-prototype/EvtProcessor  -no_cleanup $* >${cmtEvtSteeringtempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtEvtSteeringtempfile}
  unset cmtEvtSteeringtempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtEvtSteeringtempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtEvtSteeringtempfile}
unset cmtEvtSteeringtempfile
return $cmtsetupstatus

