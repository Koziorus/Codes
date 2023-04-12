function [value] = compute_rocket_time(t)
    m_0 = 150000;
    q = 2700;
    u = 2000;

    v = 750;

    value = u * ln(m_0 / (m_0 - q * t)) - g * t - v;
    return
end