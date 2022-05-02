clear; clc; close all;

%% Instantiate camera grabber

% Replace here the IP address of the IP camera
IP_ADDRESS = "192.168.87.42";
url = 'http://' + IP_ADDRESS + ':8080/video';
% Create the object (replace here your own username and password)
USERNAME = "andrea";
PASSWORD = "computervision";
cam = ipcam(url, USERNAME, PASSWORD);
% Preview (uncomment next line to test communication)
% preview(cam)

%% Live display of continuosly acquired frames

% Create the figure
% for i =1:10
% figure(i);
% picture = snapshot(cam);
% AR = size(picture,2) / size(picture,1);
% picture = imresize(picture,[480, 480 * AR]);
% fig = imshow(picture);
% [imagePoints,boardSize] = detectCheckerboardPoints(picture);
% if imagePoints > 10
% name = sprintf('checkerBoard_%d',i);
% saveas(gcf,name,'png');
%
% end
% pause(3);
%
% end
figure();
picture = snapshot(cam);
AR = size(picture,2) / size(picture,1);
picture = imresize(picture,[480, 480 * AR]);
fig = imshow(picture);
% Update the image with the latest frame until window is closed
nframes = 0;
i = 0;
while true
    try
       

        picture = snapshot(cam);
        picture = imresize(picture,[480, 480 * AR]);
        set(fig, 'CData', picture);
        title(string(nframes))
        drawnow;
        nframes = nframes + 1;
        if mod(nframes, 100) == 0
            [imagePoints,boardSize] = detectCheckerboardPoints(picture);
            

            if length(imagePoints) == 30
                i=i+1;
               
                name = sprintf('checkerBoard_%d',i);
                saveas(gcf,name,'png');
                imageName = sprintf('%s.png', name);

                
                I = imread(imageName);
                J = insertText(I,imagePoints,1:size(imagePoints,1));
                K = insertMarker(J,imagePoints,'o','Color','red','Size',5);
                imshow(K);
                title(sprintf('Detected a %d x %d Checkerboard',boardSize));
                

            end
            if i == 15
                break;
            end



        end
    catch ME
        if ~strcmp(ME.identifier, 'MATLAB:class:InvalidHandle')
            close all
            error(ME.message)
        end
        break;
    end
end

%% Close communication
clear cam