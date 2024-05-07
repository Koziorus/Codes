#include "sorting.h"

// dynamic fib 
int fib(int n)
{
    if (n == 0)
    {
        return 0;
    }

    if (n == 1)
    {
        return 1;
    }

    if (fib_arr[n] == 0)
    {
        fib_arr[n] = fib(n - 1) + fib(n - 2);
    }

    return fib_arr[n];
}

int sort_phase(Tape* in_A, Tape* in_B, Tape* out, int phase_index, const char* carry_record, int* dummy_runs)
{
    Tape* in_tapes[] = { in_A, in_B };

    // single records for tape in_A and in_B 
    const char records[DISTR_TAPE_NUM][MAX_RECORD_SIZE];
    // single records that were previously read for tape in_A and in_B
    const char prev_records[DISTR_TAPE_NUM][MAX_RECORD_SIZE];

    read_from_tape(in_tapes[TAPE_IN_A], records[TAPE_IN_A]);

    if (phase_index == 0)
    {
        int in_b_tape_status = read_from_tape(in_tapes[TAPE_IN_B], records[TAPE_IN_B]);

        // an exception when there's only one run after initial distribution (then the smaller tape has no runs)
        if (in_b_tape_status == EMPTY_TAPE)
        {
            // flush in_A to out
            int in_a_tape_status = 0;
            while (in_a_tape_status != EMPTY_TAPE)
            {
                write_to_tape(out, records[TAPE_IN_A]);
                strcpy(prev_records[TAPE_IN_A], records[TAPE_IN_A]);
                in_a_tape_status = read_from_tape(in_tapes[TAPE_IN_A], records[TAPE_IN_A]);
            }
            return SORT_END;
        }
    }
    else
    {
        strcpy(records[TAPE_IN_B], carry_record);
    }

    // setting the previous records to the current records as base case
    // this may be unnecessary 
    strcpy(prev_records[TAPE_IN_A], records[TAPE_IN_A]);
    strcpy(prev_records[TAPE_IN_B], records[TAPE_IN_B]);

    while (1)
    {
        int min_id;
        if ((*dummy_runs) == 0)
        {
            min_id = (get_index(records[TAPE_IN_A]) < get_index(records[TAPE_IN_B]) ? TAPE_IN_A : TAPE_IN_B);
        }
        else
        {
            min_id = TAPE_IN_B;
        }

        write_to_tape(out, records[min_id]);

        strcpy(prev_records[min_id], records[min_id]);
        int min_tape_status = read_from_tape(in_tapes[min_id], records[min_id]);

        if (get_index(prev_records[min_id]) > get_index(records[min_id]) || min_tape_status == EMPTY_TAPE)
        {
            if ((*dummy_runs) == 0)
            {
                // write from the NOT min tape to out unitl the end of the run
                // !min_id identifies the tape and its record that is not minimum
                int not_min_id = !min_id;
                int not_min_tape_status = 0;
                while (not_min_tape_status != EMPTY_TAPE && get_index(prev_records[not_min_id]) <= get_index(records[not_min_id]))
                {
                    write_to_tape(out, records[not_min_id]);
                    strcpy(prev_records[not_min_id], records[not_min_id]);
                    not_min_tape_status = read_from_tape(in_tapes[not_min_id], records[not_min_id]);
                }

                // this is so that prev_records and records is now in the same run
                strcpy(prev_records[not_min_id], records[not_min_id]);

                // if end of not_min_id tape
                if (not_min_tape_status == EMPTY_TAPE)
                {
                    strcpy(carry_record, records[TAPE_IN_A]);
                    if (not_min_tape_status == EMPTY_TAPE && min_tape_status == EMPTY_TAPE)
                    {
                        return SORT_END;
                    }
                    else
                    {
                        return SORT_CONTINUE;
                    }
                }
            }
            else
            {
                (*dummy_runs)--;
            }

            // when a new run will begin the prev_record and record will have the same value
            strcpy(prev_records[min_id], records[min_id]);
        }

        if (min_tape_status == EMPTY_TAPE)
        {
            strcpy(carry_record, records[TAPE_IN_A]);
            return SORT_CONTINUE;
        }

    }
}

int initial_distribution(Tape* in, Tape* out_a, Tape* out_b, int* bigger_tape_index, int* overall_runs)
{
    Tape* out[DISTR_TAPE_NUM] = { out_a, out_b };

    // last records of runs in tapes
    const char last_records[DISTR_TAPE_NUM][MAX_RECORD_SIZE];

    // carry record from reading a run
    const char carry_record[MAX_RECORD_SIZE] = "empty";

    int runs_num[DISTR_TAPE_NUM] = { 0, 0 };

    // distribute all runs
    for (int i = 0; 1; i++)
    {
        // alternate between tapes
        for (int out_tape_index = 0; out_tape_index < DISTR_TAPE_NUM; out_tape_index++)
        {
            // transfer fib(n) runs to a tape
            // fib((2 * i) + 1 + out_tape_index -> for tape out_a: 1 2 5 13 ; for tape out_b: 1 3 8 21
            while (runs_num[out_tape_index] < fib((2 * i) + 1 + out_tape_index))
            {
                // start record of a run
                const char start_record[MAX_RECORD_SIZE];

                const char record[MAX_RECORD_SIZE];
                const char prev_record[MAX_RECORD_SIZE];

                int tape_flag = 0;

                if (strcmp(carry_record, "empty") == 0)
                {   
                    tape_flag = read_from_tape(in, record);
                }
                else
                {
                    strcpy(record, carry_record);
                }

                if (tape_flag != EMPTY_TAPE)
                {
                    strcpy(prev_record, record);
                    strcpy(start_record, record);

                    for (int j = 0; tape_flag != EMPTY_TAPE && get_index(prev_record) <= get_index(record); j++)
                    {
                        write_to_tape(out[out_tape_index], record);

                        strcpy(prev_record, record);

                        tape_flag = read_from_tape(in, record);
                    }

                    // if the next run doesn't coalesce
                    if (runs_num[out_tape_index] == 0 || get_index(start_record) < get_index(last_records[out_tape_index]))
                    {
                        runs_num[out_tape_index]++;

                        // NOTE: runs after coalescing
                        (*overall_runs)++;
                    }

                    strcpy(carry_record, record);
                    strcpy(last_records[out_tape_index], prev_record);
                }

                if (tape_flag == EMPTY_TAPE)
                {
                    int dummy_runs_num = fib((2 * i) + 1 + out_tape_index) - runs_num[out_tape_index];
                    (*bigger_tape_index) = out_tape_index;

                    printf("Runs:\n\tin_A: %d(%d)\n\tin_B: %d\n", runs_num[(*bigger_tape_index)] + dummy_runs_num, dummy_runs_num, runs_num[!(*bigger_tape_index)]);
                    flush_block_buffer(out_a->block_buffer);
                    flush_block_buffer(out_b->block_buffer);

                    return dummy_runs_num;
                }
            }
        }
    }
}


int sort_tapes(Tape* tapes[TAPE_NUM], int dummy_runs_num_in)
{
    int dummy_runs_num = dummy_runs_num_in;

    char carry_record[MAX_RECORD_SIZE];
    int sort_flag = SORT_CONTINUE;
    Tape* inA, * inB, * out, * prev_out;

    int phase_index = 0;
    while (sort_flag == SORT_CONTINUE)
    {
        printf("\tPhase: %d\n", phase_index + 1);
        // NOTE: pattern: (for the inA (assuming that it is the longest tape): t1,t3,t2,t1,t3,... (for the inB): t2,t1,t3,t2,t1,...(for the out): t3,t2,t1,t3,t2,...
        inA = tapes[(TAPE_NUM - (phase_index % TAPE_NUM)) % TAPE_NUM];
        inB = tapes[(TAPE_NUM - ((phase_index + 2) % TAPE_NUM)) % TAPE_NUM];
        out = tapes[(TAPE_NUM - ((phase_index + 1) % TAPE_NUM)) % TAPE_NUM];
        prev_out = inA; // tapes[(T - ((p + 1 - 1) % T)) % T] = tapes[(T - (p % T)) % T]

        // NOTE: dont change the inB to READ (because it will wipe out the current buffer) (we know though that inB is already in READ mode)
        tape_set_mode(prev_out, READ);
        tape_set_mode(out, WRITE);

        sort_flag = sort_phase(inA, inB, out, phase_index, carry_record, &dummy_runs_num);
        flush_block_buffer(out->block_buffer);

#ifdef PRINT_AFTER_PHASE
        tape_set_mode(out, READ);
        printf("Tape 1:");
        print_tape(out->block_buffer->assigned_file_path);
        printf("Tape 2:");
        print_tape(out->block_buffer->assigned_file_path);
        printf("Tape 3:");
        print_tape(out->block_buffer->assigned_file_path);
#endif

        phase_index++;
    }

    return phase_index; // HERE: phase_index is the number of phases
}