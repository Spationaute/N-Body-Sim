resultsFiles=ls("results");
n=length(resultsFiles);
cur=0;
for ii=1:n
    fileName = strtrim(resultsFiles(ii,:));
    if(strncmp(fileName,"data",4));
        printf("Importing '%s'\n",fileName);
        cur += 1;
        time(cur) = str2num(strsplit(fileName,"."){2});
        data(cur,:,:) = csvread(["results/" fileName]);
    end;
end;
[_,order]=sort(time);
ii=0;
xl=([min(min(data(:,:,1))),max(max(data(:,:,1)))])
yl=([min(min(data(:,:,2))),max(max(data(:,:,2)))])
zl=([min(min(data(:,:,3))),max(max(data(:,:,3)))])
while true
    ii = mod(ii + 1,length(data));
    t = order(ii+1);
    scatter3(data(t,2:end,1),data(t,2:end,2),data(t,2:end,3))
    title(sprintf("Time %i",t));
    xlim(xl);
    ylim(yl);
    if zl(2)>0;
        zlim(zl);
    end;
    pause(0.05);
endwhile
