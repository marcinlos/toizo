#!/usr/bin/env python

import sys
import getopt
from subprocess import Popen, PIPE
from math import log10
import re

from os import kill
from signal import alarm, signal, SIGALRM, SIGKILL, SIGQUIT
from reportlab.graphics.charts.legends import TotalAnnotator

def run(args, cwd=None, shell=False, kill_tree=True, timeout=-1, env=None):
    class Alarm(Exception): pass
    
    def alarm_handler(signum, frame):
        raise Alarm
    
    p = Popen(args, shell=shell, cwd=cwd, stdout=PIPE, stderr=PIPE, env=env)
    if timeout != -1:
        signal(SIGALRM, alarm_handler)
        alarm(timeout)
        pass
    try:
        stdout, stderr = p.communicate()
        if timeout != -1:
            alarm(0)
    except Alarm:
        try: 
            kill(p.pid, SIGKILL)
        except OSError:
            pass
        return -9, '', ''
    return p.returncode, stdout, stderr


optlist, args = getopt.getopt(sys.argv[1:], 'n:w:')
argv = {o: v for o, v in optlist}


times = int(argv['-n'])
timeout = int(argv.get('-w', '5'))

cmd = 'bin/solver {}'.format(' '.join(args))

class Stats(object):
    def __init__(self):
        self.values = []
        self.fails = 0
    
    def success(self, calls):
        self.values.append(calls)
        
    def failed(self):
        self.fails += 1
                
    def display(self):
        total = len(self.values) + self.fails
        ok = len(self.values)
        fail = self.fails
        ok_perc = ok / float(total)
        fail_perc = fail / float(total)
        print 'Total:     {:>5}'.format(total)
        print 'Success:   \033[1;32m{:>5}    ({})\033[0m'.format(ok, ok_perc)
        print 'Failed:    \033[1;31m{:>5}    ({})\033[0m'.format(fail, fail_perc)
        print 'Avg:   {:>9}'.format(sum(self.values) / len(self.values))
    
stats = Stats()

def quit_handler(signum, frame):
    sys.stdout.write('\r')
    stats.display()
    
signal(SIGQUIT, quit_handler)

pattern = re.compile('Calls: (\d+)\nSolution: (YES|NO)', re.MULTILINE)
try:
    for _ in xrange(0, times):
        code, _, out = run(cmd, shell=True, timeout=timeout)
        print '\rcalls: ',
        if code == 0:
            match = pattern.match(out)
            calls, solved = match.groups()
            print '{:>10}      (solution: {:>3})'.format(calls, solved)
            stats.success(int(calls))
        else:    
            print '\033[1;31m{:>10}\033[0m'.format('TIMEOUT')
            stats.failed()
            
except KeyboardInterrupt:
    pass

print
stats.display()
