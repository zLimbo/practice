package com.zlimbo.study.leetcode;

import java.util.Arrays;
import java.util.List;

class LCP31变换的迷宫 {

    class Solution {

        public int search(int[] nums, int target) {
            return (int) Arrays.stream(nums).parallel().filter(
                    n -> n== target
            ).count();
        }

        public boolean escapeMaze(List<List<String>> maze) {
            int T = maze.size(), n = maze.get(0).size(), m = maze.get(0).get(0).length();

            boolean[][][][] dp = new boolean[T][n][m][5];
            boolean[][][] can = new boolean[n][m][2];

            for (int t = 0; t < T; ++t) {
                for (int k = 0; k < 4; ++k) {
                    dp[t][n - 1][m - 1][k] = true;
                }
            }

            can[n - 1][m - 1][0] = can[n - 1][m - 1][1] = true;

            int[] dx = {1, -1, 0, 0, 0};
            int[] dy = {0, 0, 1, -1, 0};

            for (int t = T - 2; t >= 0; --t) {
                for (int i = 0; i < n; ++i) {
                    for (int j = 0; j < m; ++j) {
                        for (int k = 0; k < 4; ++k) {
                            int u = k & 1, v = k & 2;
                            for (int d = 0; d < 5; ++d) {
                                int ni = i + dx[d], nj = j + dy[d];
                                if (0 <= ni && ni < n && 0 <= nj && nj < m) {
                                    if (maze.get(t + 1).get(ni).charAt(nj) == '.') {
                                        dp[t][i][j][k] |= dp[t + 1][ni][nj][k];
                                    } else {
                                        if (0 == u) {
                                            dp[t][i][j][k] |= dp[t + 1][ni][nj][k | 1];
                                        }
                                        if (0 == v) {
                                            dp[t][i][j][k] |= can[ni][nj][u];
                                        }
                                    }
                                }
                            }
                        }
                        can[i][j][0] |= dp[t][i][j][2];
                        can[i][j][1] |= dp[t][i][j][3];
                    }
                }
            }

            return dp[0][0][0][0];
        }
    }
}