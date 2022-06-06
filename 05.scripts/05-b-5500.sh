#!/bin/bash

cat /etc/passwd | awk -F : 'BEGIN {printf("<table>\n <tr>\n <th>%s</th>\n <th>%s</th>\n <th>%s</th>\n <th>%s</th>\n </tr>\n", "Username", "Group", "Login Shell", "GECKO") } { printf(" <tr>\n <td>%s</td>\n <td>%s</td>\n <td>%s</td>\n <td>%s</td>\n </tr>\n", $1, $4, $7, "GECKO") } END { printf("</table>\n") }'
