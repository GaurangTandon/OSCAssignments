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
start = 3

while pointer < len(s):
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

    for p in range(start,start+proccount):
        if changes.get(p):
            queue[p-start].append(changes[p])
        else:
            queue[p-start].append(queue[p-start][-1])
    hits.append(tckstart)
print(hits)
plt.yticks([0,1,2,3,4])

c=start
plt.xlabel("Ticks")
plt.ylabel("Queue number")
for x in queue:
    plt.plot(x, linestyle='-', marker='o', label=str(c))
    c+=1
plt.legend()
plt.show()
