import heapq

class Node:
    __slots__ = ("point", "axis", "left", "right")
    def __init__(self, point, axis):
        self.point, self.axis = point, axis
        self.left = self.right = None

def build(points, depth=0):
    if not points:
        return None
    k = len(points[0])
    axis = depth % k
    points.sort(key=lambda p: p[axis])
    mid = len(points) // 2
    node = Node(points[mid], axis)
    node.left  = build(points[:mid], depth + 1)
    node.right = build(points[mid + 1:], depth + 1)
    return node

def dist2(a, b):
    return sum((x - y) ** 2 for x, y in zip(a, b))

def knn(node, query, k, heap, root_axis=0):
    if node is None:
        return
    d = dist2(query, node.point)
    if len(heap) < k:
        heapq.heappush(heap, (-d, node.point))     # max-heap via negation
    elif d < -heap[0][0]:
        heapq.heapreplace(heap, (-d, node.point))

    axis = node.axis
    delta = query[axis] - node.point[axis]
    near, far = (node.left, node.right) if delta < 0 else (node.right, node.left)

    knn(near, query, k, heap)

    if len(heap) < k or delta * delta < -heap[0][0]:
        knn(far, query, k, heap)

# usage
root = build([(2,3),(5,4),(9,6),(4,7),(8,1),(7,2),(11,2),(17,8),(12,7),(3,5),(0,9),(14,11),(9,1)])
heap = []
knn(root, (9,2), 3, heap)
neighbors = sorted(((-d, p) for d, p in heap))     # ascending distance
print(neighbors)