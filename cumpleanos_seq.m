clear, clc
%% C feliz
melodia=[128, 60 60 62 60 65 64, 60 60 62 60 67 65, 60 60 72 69 65 64 62, 70 70 69 65 67 65 128];
duracion=[16, 12 6 16 16 16 32, 12 6 16 16 16 32, 12 6 16 16 16 16 16, 12 6 16 16 16 32 48];

%% Feliz en tu dia
melodia=[128, 65 67 67, 69 67 69, 70 65, 128 65 65, 67 67 67, 69 67 69, 70 65, 128 65, 63 62 63, 67 65 63, 62 65, 128 70 72, 74 74, 72 72 70, 128]; 
duracion=[1, 1, 2 1, 1 1 1, 1 2, 1 1 1, 1 1 1, 1 1 1, 1 3, 1 1,  1 1 1, 1 1 1, 1 2, 1 1 1 , 2 1, 2 1 ,5, 3 ];
duracion = duracion*16;


%% 
melodia=[128 69 69 69 69 69 69 69 72 65 67 69, 70 70 70 70 69 69 69 67 67 69 67 72, 69 69 69 69 69 69 69  72 65 67 69, 70 70 70 70 69 69 72 72 70 67 65, 60 69 67 65 60 60 69 67 65 62, 62 70 69 67 64 64 72 72 70 67 69, 60 69 67 65 60 60 69 67 65 62, 62 70 69 67 72 72 72 74 72 70 67 65];
duracion=[4 1 1 2 1 1 2 1 1 1 1 4, 1 1 2 1 1 2 1 1 1 1 2 2, 1 1 2 1 1 2 1 1 1 1 4, 1 1 2 1 1 2 1 1 1 1 4, 1 1 1 1 4 1 1 1 1 4, 1 1 1 1  3 1 1 1 1 1 4, 1 1 1 1 4 1 1 1 1 4, 1 1 1 1 2 1 1 1 1 1 1 4];
duracion =duracion*8;

%% 
%%duracion=duracion*0.5;
silencio=1; %%Semifusas
salida=0;
longitud=sum(duracion)+(length(duracion)-1)*silencio;

i=1;
for k=1:length(melodia)
    for j=1:duracion(k)
        salida(i)=melodia(k);
        i=i+1;
    end
    if k<length(melodia)
        for l=1:silencio
            salida(i)=128;
            i=i+1;
        end
    end
end
salida(i)=255;

curva='{ ';
for k=1:length(salida)-1
    curva = [curva, num2str(salida(k)), ', '];
end
curva=[curva, num2str(salida(end)), ' };'];

curva

%% Run-length encoding algorithm 
%duracion=duracion*0.5;
silencio=1; %%Semifusas
melodia2=0;
duracion2=0;
longitud=sum(duracion)+(length(duracion)-1)*silencio;

i=1;
for k=1:length(melodia)
       melodia2(i)=melodia(k);
        i=i+1;   
        if k<length(melodia)&& melodia(k)~=128          
            melodia2(i)=128;
            i=i+1;       
        end
end
melodia2(i)=255;

i=1;
for k=1:length(duracion)
       duracion2(i)=duracion(k);
        i=i+1;   
        if k<length(duracion) && melodia(k)~=128       
            duracion2(i)=silencio;
            i=i+1;       
        end
end
duracion2(i)=silencio;

curvam='{ ';
for k=1:length(melodia2)-1
    curvam = [curvam, num2str(melodia2(k)), ', '];
end
curvam=[curvam, num2str(melodia2(end)), ' };'];

curvam

curvad='{ ';
for k=1:length(duracion2)-1
    curvad = [curvad, num2str(duracion2(k)), ', '];
end
curvad=[curvad, num2str(duracion2(end)), ' };'];

curvad


%% 
fref=440;
offset=10;

k=1:1:17;
f=fref*2.^((k-offset)./12);

periods=round(1e6./(8*f));

periods
    
f2=125000./periods

periodsH=floor(periods/256);
periodsL=rem(periods, 256);

curvaH='{ ';
for k=1:length(periodsH)-1
    curvaH = [curvaH, num2str(periodsH(k)), ', '];
end
curvaH=[curvaH, num2str(periodsH(end)), ' };'];

curvaL='{ ';
for k=1:length(periodsL)-1
    curvaL = [curvaL, num2str(periodsL(k)), ', '];
end
curvaL=[curvaL, num2str(periodsL(end)), ' };'];

curvaL

curvaH