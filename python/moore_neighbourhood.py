def count_neighbours(grid, row, col):
	cnt = 0
	for dr in [-1, 0, 1]:
		for dc in [-1, 0, 1]:
			nr = row + dr
			nc = col + dc
			# nr, nc都在范围内 且 同时大于等于0 且 dr, dc不同时为0
			if nr < len(grid) and nc < len(grid[0]):
				if (nr >= 0 and nc >= 0):
					if not(dr == 0 and dc == 0):
						if grid[nr][nc] == 1:
							cnt += 1
							
    return cnt

	#return sum(sum(grid[x][max(0,col-1):min(len(grid),col+2)]) for x in range(max(0,row-1),min(len(grid),row+2))) - grid[row][col]

if __name__ == '__main__':
    #These "asserts" using only for self-checking and not necessary for auto-testing
    assert count_neighbours(((1, 0, 0, 1, 0),
                             (0, 1, 0, 0, 0),
                             (0, 0, 1, 0, 1),
                             (1, 0, 0, 0, 0),
                             (0, 0, 1, 0, 0),), 1, 2) == 3, "1st example"
    assert count_neighbours(((1, 0, 0, 1, 0),
                             (0, 1, 0, 0, 0),
                             (0, 0, 1, 0, 1),
                             (1, 0, 0, 0, 0),
                             (0, 0, 1, 0, 0),), 0, 0) == 1, "2nd example"
    assert count_neighbours(((1, 1, 1),
                             (1, 1, 1),
                             (1, 1, 1),), 0, 2) == 3, "Dense corner"
    assert count_neighbours(((0, 0, 0),
                             (0, 1, 0),
                             (0, 0, 0),), 1, 1) == 0, "Single"
