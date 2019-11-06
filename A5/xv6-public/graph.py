import matplotlib.pyplot as plt

# use statically defined values lol
# each row represents queue values of a particular process
procTimestamps = [
    [0, 1, 1, 0, 0, 0, 0, 0, 0, 0, ],
    [0, 2, 0, 3, 0, 0, 0, 0, 0, 0, ],
    [0, 1, 2, 1, 2, 2, 2, 1, 1, 0, ],
    [0, 1, 1, 1, 3, 1, 0, 0, 0, 0, ],
    [0, 0, 3, 2, 1, 1, 0, 0, 0, 0, ],
    [0, 0, 1, 2, 2, 2, 1, 1, 1, 1, ],
    [0, 2, 1, 0, 0, 0, 0, 0, 0, 0, ],
    [0, 3, 3, 1, 0, 0, 0, 0, 0, 0, ],
    [0, 2, 0, 0, 0, 0, 0, 0, 0, 0, ]
]

plt.yticks([0,1,2,3,4])
for x in procTimestamps:
    plt.plot(x, linestyle='-', marker='o')
plt.show()
