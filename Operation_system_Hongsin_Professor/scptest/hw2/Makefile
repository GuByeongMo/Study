# 타겟 실행 파일
TARGETS = gamatch rand_agent timeout_agent greedy_agent

# 기본 빌드 명령
all: $(TARGETS)

# 각 타겟 빌드 방법
gamatch: gamatch.c
	gcc -Wall -Wextra -O2 -o gamatch gamatch.c

rand_agent: rand_agent.c
	gcc -Wall -Wextra -O2 -o rand_agent rand_agent.c

timeout_agent: timeout_agent.c
	gcc -Wall -Wextra -O2 -o timeout_agent timeout_agent.c

greedy_agent: greedy_agent.c
	gcc -Wall -Wextra -O2 -o greedy_agent greedy_agent.c

# 빌드 결과물 제거
clean:
	rm -f $(TARGETS)
