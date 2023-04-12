function [value] = compute_time(N)
    value = ((N^(1.43) + N^(1.14)) / 1000) - 5000;
    return
end