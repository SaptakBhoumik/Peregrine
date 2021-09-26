def main():
int n1=0
int n2 = 1
int nth=0
int count = 0
int nterms=45
    while count < nterms:
        nth = n1 + n2
        n1 = n2
        n2 = nth
        count ++