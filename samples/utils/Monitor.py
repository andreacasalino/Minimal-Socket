import subprocess
import threading
import tempfile
import os
import sys
from io import StringIO 
from optparse import OptionParser
import time

def onlyOnceTrue(iterable):
    firstTime = True
    for element in iterable:
        yield firstTime, element
        firstTime = False

class ProcessHandler:
    def __init__(self, cmd_line, location, sleep_initial=None):
        if not sleep_initial == None:
            # print('sleeping {} [s]'.format(sleep_initial))
            time.sleep(float(sleep_initial))
        self.cmd = cmd_line.split()
        if not location == None:
            self.cmd[0] = os.path.join(location, self.cmd[0])
        self.thread = threading.Thread(target=self.run_)
        self.thread.start()
        self.stdout = ''
        self.stderr = ''

    def __str__(self):
        return ' '.join([os.path.basename(piece) if isFirst else piece for isFirst, piece in onlyOnceTrue(self.cmd)])

    def run_(self):
        hndlr = subprocess.Popen(self.cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
        self.stdout, self.stderr = hndlr.communicate()
        if not hndlr.returncode == 0:
            msg = 'Something wrong from {}: {}'.format(self.cmd[0], self.stderr)
            raise Exception(msg)

    def get(self):
        if not self.thread == None:
            self.thread.join()
            self.thread = None
        return self.stdout, self.stderr
    
    def fromFile(source, location=None, sleep_in_between=None):
        with open(source, 'r') as stream:
            return [ProcessHandler(line.strip(), location, None if firstTime else sleep_in_between) for firstTime, line in onlyOnceTrue(stream.readlines())]

class Monitor:
    LAYOUT_TEMPLATE="""
<html>
    <head>
        <style>
            .shell {
                color: antiquewhite;
                border-left: 1px solid antiquewhite;
                border-right: 1px solid antiquewhite;
                height: 100%;
                padding-left: 5px;
                padding-right: 5px;
            }
            .cmd {
                color: pink;
                font-weight: bold;
            }
        </style>
    </head>
    <body style="background-color: black;">
        <div style="display: flex;">
            $SHELLS
        </div>
    </body>
</html>
"""

    SHELL_TEMPLATE = """
            <div class="shell" style="width: {WIDTH}%;height: 100%; flex: 1">
                <div class="cmd"> {CMD} </div>
                <hr>
{LINES}
            </div>    
"""

    def makeShell(process, w):
        out, err = process.get()        
        shell_content  = '\n'.join(['<div class="stdout">{}</div>'.format( line.strip() ) for line in StringIO(out).readlines()])
        shell_content += '\n'.join(['<div class="stderr">{}</div>'.format( line.strip() ) for line in StringIO(err).readlines()])
        return Monitor.SHELL_TEMPLATE.format(**{
            'WIDTH': w,
            'CMD': str(process),
            'LINES': shell_content
        })

    def make(cmd_source, destination, location=None, sleep_in_between=None):
        procs = ProcessHandler.fromFile(cmd_source, location, sleep_in_between)    
        w = int(round(100 / len(procs), 0))
        shells = '\n'.join([Monitor.makeShell(proc, w) for proc in procs])
        with open(destination, 'w') as stream:
            stream.write(Monitor.LAYOUT_TEMPLATE.replace('$SHELLS', shells))

def test():
    tmpdirname = tempfile.TemporaryDirectory()
    tempFile = os.path.join(tmpdirname.name, 'Commands')
    with open(tempFile, 'w') as stream:
        stream.write('ls\n')
        stream.write('ls ../')
    output = os.path.join( os.path.dirname(__file__) , 'test.html')
    output = os.path.abspath(output)
    Monitor.make(tempFile, output)
    print('open in a browser {}'.format(output))

def monitor(options):
    Monitor.make(options.cmd, options.dest, options.location, options.sleep)
    print('\nopen in a browser {} to see the results sent by the run processes'.format(options.dest))

def main():
    parser = OptionParser()
    parser.add_option("--location", default=None, help="prefix of the path storing the processes to run")
    parser.add_option("--sleep", default=None, help="intra start of processes sleep in [s]")

    parser.add_option("--test", action="store_true", default=False)

    parser.add_option("--cmd", default=None, help="files with the list of commands")
    parser.add_option("--dest", default=None, help="the location where to generate the hmtl page with the stdout of all processes")

    (options, args) = parser.parse_args()

    if options.test:
        test() 
    elif options.cmd and options.dest:
        monitor(options)
    elif options.cmd:
        # just show the commands
        print()
        with open(options.cmd, 'r') as stream:
            print(''.join(['Running {}'.format(line) for line in stream.readlines()]))
        print('\n\nwaiting for all the spawned processes to complete ...\n\n')

if __name__ == '__main__':
    main()
