 {
         double *x = new double[2];
         double *y = new double[2];
         double *z = new double[2];
         x[0] = 6215.;
         x[1] = 5542.;
         y[0] = 3853.;
         y[1] = 5270.;
         z[0] = 2723.;
         z[1] = 2724.;
         TGraph2D * g = new TGraph2D(2, x, y, z);
         //g->Draw("LINE");   
         g->Draw("tri1 p0");   
}                                           }
