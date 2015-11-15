function v = linfit(y, dt)
    x_avg = 0;
    x_sum = 0;
    y_avg = 0;
    y_sum = 0;
    num = 0;
    den = 0;
    n = length(y);
    x = zeros(n);
    
    for i = 1:n
        x(i) = i * dt;
    end
    
    for i = 1:n
        x_sum += x(i);
    end
    
    x_avg = x_sum/n;
    
    for i = 1:n
       y_sum += y(i);
    end
    
    y_avg = y_sum / n;
    
    % Calculate numerator
    for i = 1:n
        num = num + ((x(i) - x_avg) * (y(i) - y_avg));
    end
    
    % Calculate denominator
    for i = 1:n
        den = den + ((x(i) - x_avg) ^ 2);
    end      
    
    v = num / den;

end