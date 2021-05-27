export JUNOTOP_OEC=/cvmfs/juno.ihep.ac.cn/sl6_amd64_gcc830/Pre-Release/J20v1r0-Pre1
export OEC_EF=/junofs/users/liuyan/OEC/oec-prototype

pushd $JUNOTOP_OEC >& /dev/null
    source setup.sh
popd >& /dev/null

pushd $OEC_EF/cmt/ >& /dev/null
    source setup.sh
popd >& /dev/null

export CMTPROJECTPATH=${WORKTOP}:${CMTPROJECTPATH}

