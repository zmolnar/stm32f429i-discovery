function h = pressure_to_height( p)
    h = 44330 * (1-(p/101325)^0.1902);
end