#!/bin/bash

source /usr/global/Modules/current/init/bash

module purge
module load gcc/4.8.2
module load intel
module load boost/1.54.0

#module load java/1.7.0_21
module load flume

module list

mkdir -p /tmp/Clpr/{logs,output,output_tmp,output_old,data,checkpoint}

dir=/gpfs/home/ivy1/Projects/Clpr/clpr_d
tdir=/tmp/Clpr

/sbin/fuser -k ${tdir}/logs/clprd.log
/sbin/fuser -k ${tdir}/logs/flume.log

rm -f ${tdir}/logs/*
rm -f ${tdir}/output/*
rm -f ${tdir}/output_tmp/*

nohup ${dir}/bin/clpr_d -c ${dir}/clprd.xml > ${tdir}/logs/c.log 2>&1 &

nohup flume-ng agent -f ${dir}/flume/2/flume.conf_spoolDir_avro  -n A2 -c ${dir}/flume/2 > ${tdir}/logs/flume.log 2>&1 &

fuser ${tdir}/logs/clprd.log
fuser ${tdir}/logs/flume.log

crontab -l > ${tdir}/crontab.clpr
cat ${dir}/flume/2/crontab.sort >> ${tdir}/crontab.clpr
sort ${tdir}/crontab.clpr | uniq > ${tdir}/crontab.clpr.1
crontab ${tdir}/crontab.clpr.1

crontab -l

