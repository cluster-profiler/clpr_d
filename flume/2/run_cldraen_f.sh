#!/bin/bash

module purge
module load gcc/4.8.2
module load boost/1.54.0
# module load flume
module load java/1.7.0_21

export SEARCH_HOME=/opt/cloudera/parcels/SOLR/lib/search

mkdir -p /tmp/Clpr/{logs,output,output_tmp,output_old,data,checkpoint}

dir=/gpfs/home/ivy1/Projects/Clpr/clpr_d

# /sbin/fuser -k /tmp/Clpr/logs/clpr.log

/sbin/fuser -k /tmp/Clpr/logs/flume.log


# nohup ${dir}/bin/clpr_d -c ${dir}/clprd.xml > /tmp/Clpr/logs/c.log 2>&1 &

# nohup flume-ng agent -f ${dir}/flume/2/flume.conf_spoolDir_avro  -n A2 -c ${dir}/flume/2 > /tmp/Clpr/logs/flume.log 2>&1 &



nohup flume-ng  agent -Xmx16384m -Xms8192m -f ${dir}/flume/2/flume.conf_avro_file  -n A3 -c ${dir}/flume/2 > /tmp/Clpr/logs/flume.log 2>&1 &
