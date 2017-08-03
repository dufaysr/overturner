outputdir = 'problem2box_a75';

M5000a = loadMbin(outputdir,'M5000a');
M5000b = loadMbin(outputdir,'M5000b');
norm(M5000a-M5000b,'fro')

M10000a = loadMbin(outputdir,'M10000a');
M10000b = loadMbin(outputdir,'M10000b');
norm(M10000a-M10000b,'fro')

M20000a = loadMbin(outputdir,'M20000a');
M20000b = loadMbin(outputdir,'M20000b');
norm(M20000a-M20000b,'fro')

M40000a = loadMbin(outputdir,'M40000a');
M40000b = loadMbin(outputdir,'M40000b');
norm(M40000a-M40000b,'fro')

M90000a = loadMbin(outputdir,'M90000a');
M90000b = loadMbin(outputdir,'M90000b');
norm(M90000a-M90000b,'fro')

M = loadMbin(outputdir,'M1500000');
H = zeros(300,2);
H(1:150,1) = 1;
H(151:300,2) = 1;
D = diag(ones(300,1)./300);
M1500000 = H'*M*D*H;