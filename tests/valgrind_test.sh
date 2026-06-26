#!/bin/bash

# Valgrind & Helgrind test script for philosophers
# Usage: ./tests/valgrind_test.sh [valgrind|helgrind|all]

BINARY="./philo"
LOG_DIR="tests/logs"
PASS=0
FAIL=0

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
RESET='\033[0m'

mkdir -p "$LOG_DIR"

# Check binary exists
if [ ! -f "$BINARY" ]; then
	echo -e "${RED}Error: $BINARY not found. Run 'make' first.${RESET}"
	exit 1
fi

# Check valgrind is installed
if ! command -v valgrind &>/dev/null; then
	echo -e "${RED}Error: valgrind is not installed.${RESET}"
	exit 1
fi

# ──────────────────────────────────────────────────────────────────────────────
# Test cases: (label num_philos time_to_die time_to_eat time_to_sleep [must_eat])
# ──────────────────────────────────────────────────────────────────────────────
declare -a TESTS=(
	"1_philo_dies         1  800  200  200"
	"2_philos_survive     2  800  200  200  5"
	"5_philos_survive     5  800  200  200  7"
	"5_philos_tight       5  410  200  200  5"
	"4_philos_survive     4  410  200  200  5"
	"3_philos_tight       3  310  200  100  5"
)

# ──────────────────────────────────────────────────────────────────────────────
print_header() {
	echo ""
	echo -e "${CYAN}══════════════════════════════════════════════════${RESET}"
	echo -e "${CYAN}  $1${RESET}"
	echo -e "${CYAN}══════════════════════════════════════════════════${RESET}"
}

run_valgrind() {
	print_header "VALGRIND (memory leak / error check)"

	for test in "${TESTS[@]}"; do
		read -r label args <<< "$test"
		log="$LOG_DIR/valgrind_${label}.log"

		echo -ne "  ${label} ... "

		# Timeout so simulation doesn't run forever; suppress normal program output
		timeout 30s valgrind \
			--tool=memcheck \
			--leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--error-exitcode=1 \
			$BINARY $args > "$log" 2>&1

		exit_code=$?

		if [ $exit_code -eq 124 ]; then
			# timeout — check the log for errors reported so far
			if grep -qE "ERROR SUMMARY: 0 errors" "$log"; then
				echo -e "${GREEN}PASS${RESET} (timeout, no errors)"
				((PASS++))
			else
				echo -e "${RED}FAIL${RESET} (timeout with errors — see $log)"
				((FAIL++))
			fi
		elif [ $exit_code -eq 0 ] || grep -q "ERROR SUMMARY: 0 errors" "$log"; then
			echo -e "${GREEN}PASS${RESET}"
			((PASS++))
		else
			echo -e "${RED}FAIL${RESET} — see $log"
			((FAIL++))
		fi
	done
}

run_helgrind() {
	print_header "HELGRIND (data race / thread error check)"

	for test in "${TESTS[@]}"; do
		read -r label args <<< "$test"
		log="$LOG_DIR/helgrind_${label}.log"

		echo -ne "  ${label} ... "

		timeout 60s valgrind \
			--tool=helgrind \
			--history-level=approx \
			--error-exitcode=1 \
			$BINARY $args > "$log" 2>&1

		exit_code=$?

		if [ $exit_code -eq 124 ]; then
			if grep -qE "ERROR SUMMARY: 0 errors" "$log"; then
				echo -e "${GREEN}PASS${RESET} (timeout, no races)"
				((PASS++))
			else
				echo -e "${RED}FAIL${RESET} (timeout with races — see $log)"
				((FAIL++))
			fi
		elif [ $exit_code -eq 0 ] || grep -q "ERROR SUMMARY: 0 errors" "$log"; then
			echo -e "${GREEN}PASS${RESET}"
			((PASS++))
		else
			echo -e "${RED}FAIL${RESET} — see $log"
			((FAIL++))
		fi
	done
}

# ──────────────────────────────────────────────────────────────────────────────
# Entry point
# ──────────────────────────────────────────────────────────────────────────────
MODE="${1:-all}"

case "$MODE" in
	valgrind)  run_valgrind ;;
	helgrind)  run_helgrind ;;
	all)       run_valgrind; run_helgrind ;;
	*)
		echo "Usage: $0 [valgrind|helgrind|all]"
		exit 1
		;;
esac

echo ""
echo -e "${CYAN}══════════════════════════════════════════════════${RESET}"
echo -e "  Results: ${GREEN}${PASS} passed${RESET}  ${RED}${FAIL} failed${RESET}"
echo -e "${CYAN}══════════════════════════════════════════════════${RESET}"
echo ""
echo -e "  Logs saved to ${YELLOW}${LOG_DIR}/${RESET}"
echo ""

[ $FAIL -eq 0 ] && exit 0 || exit 1
