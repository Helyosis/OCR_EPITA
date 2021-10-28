        for (int i_rho = 0; i_rho < Nrho; ++i_rho) {
            if (hough_accumulator[Ntheta * i_theta + i_rho]) {
                lines[line_index].theta = i_theta * dtheta;
                lines[line_index].rho = i_rho * drho;
                ++line_index;;
            }
        }
    }
    for(int i = 0; i < line_index; ++i) {
        double theta = lines[i].theta;
        double rho   = lines[i].rho;
        double a = cos(theta);
        double b = sin(theta);
        double x0 = a*rho;
        double y0 = b*rho;
        int x1 = clamp(x0 + 1000*(-b), 0, source->w - 1);
        int y1 = clamp(y0 + 1000*(a),  0, source->h - 1);
        int x2 = clamp(x0 - 1000*(-b), 0, source->w - 1);
        int y2 = clamp(y0 - 1000*(a),  0, source->h - 1);
        drawLine(source, x1, y1, x2, y2, 0xff00ffff);
    }
}
