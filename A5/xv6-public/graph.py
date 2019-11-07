import matplotlib.pyplot as plt

with open("/home/gt/IIIT/OSC/Assignments/A5/xv6-public/in.txt") as f:
    s = map(lambda x : [int(x[0]), int(x[1]), int(x[2])], map(lambda x : x.split(" "), f.read().split("\n")))


s = list(s)
proccount = 5
queue = []
for i in range(proccount):
    queue.append([0])

tckstart=s[0][0]
pointer = 0
hits=[]

while pointer < len(s):
    if s[pointer][1] <= 3:
        pointer+=1
        continue

    while tckstart<s[pointer][0]:
        for q in queue:
            q.append(q[-1])
        tckstart+=1

    changes = {}
    while pointer < len(s) and tckstart==s[pointer][0]:
        changes[s[pointer][1]]=s[pointer][2]
        pointer += 1

    print(tckstart, changes)

    tckstart+=1

    for p in range(4,4+proccount):
        if changes.get(p):
            queue[p-4].append(changes[p])
        else:
            queue[p-4].append(queue[p-4][-1])
    hits.append(tckstart)
print(hits)
plt.yticks([0,1,2,3,4])
for x in queue:
    plt.plot(x, linestyle='-', marker='o')
plt.show()
