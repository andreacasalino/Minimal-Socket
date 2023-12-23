import re, os, subprocess, sys, argparse

class TestsRunner:
    FOLDER = os.path.dirname(__file__)
    FOLDER = os.path.join(FOLDER, '..', '..', 'tests')

    def gatherLabels_(self):
        res = set()
        for filename in filter(lambda f: os.path.splitext(f)[1] == '.cpp' , os.listdir(TestsRunner.FOLDER)):
            with open(os.path.join(TestsRunner.FOLDER, filename), 'r') as stream:
                for m in re.finditer("""TEST_CASE\(\"(.*?)\", \"\[(.*?)\]\"""", stream.read()):                
                    res.add(m.group(2))
        self.labels = [r for r in res]

    def sortLabels_(self, first):
        sorted = []
        for label in first:
            if not label in self.labels:
                msg = '[{}] is not a valid label'.format(label)
                raise Exception(msg)
            sorted.append(label)
            self.labels.remove(label)
        for label in self.labels:
            sorted.append(label)
        self.labels = sorted

    def __init__(self, first=None):
        self.gatherLabels_()
        self.sortLabels_([] if first == None else first)

    def run(self, binLocation, fltr):
        for label in filter(lambda label: not label in fltr, self.labels):
            print('============================================')
            print('running [{}]'.format(label))
            print('============================================')
            hndlr = subprocess.Popen([binLocation, '[{}]'.format(label)], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            out, err = hndlr.communicate()            
            print('{}\n{}'.format(out, err))
            if not hndlr.returncode == 0:
                sys.exit(1)
    
if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--bin', default=None)
    parser.add_argument('--priority', default=None)
    parser.add_argument('--filter', default=None)
    args = parser.parse_args()

    if args.priority == None:
        args.priority = []
    else:
        args.priority = args.priority.split()
    if args.filter == None:
        args.filter = []
    else:
        args.filter = args.filter.split()

    if args.bin == None:
        print('labels detected at {}'.format(TestsRunner.FOLDER))
        print('\n'.join(TestsRunner().labels))
    else:
        runner = TestsRunner(args.priority)
        runner.run(args.bin, args.filter)
