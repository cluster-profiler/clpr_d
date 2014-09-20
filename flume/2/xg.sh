#!/bin/bash

module load perl

perl -e 'for($i=1;$i<=192;$i++){if($i==81){next;} $com="ssh -t lionxg${i} sudo -u root /gpfs/home/ivy1/Projects/Clpr/clpr_d/flume/2/run_nodes.sh"; print $com,"\n",`$com`}'

# ssh -t lionxg8 sudo -u root touch /tmp/z1
