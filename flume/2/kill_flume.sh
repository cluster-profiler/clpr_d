#!/bin/bash

module load perl

perl -e 'for($i=1;$i<=192;$i++){if($i==81){next;} $com="ssh lionxg${i} /sbin/fuser -k /tmp/Clpr/logs/flume.log"; print $com,"\n",`$com`}'

