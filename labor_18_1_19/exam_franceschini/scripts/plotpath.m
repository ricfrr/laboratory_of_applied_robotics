close('all');

arena_limit = [0 1.5; 0 1];

figure; hold("on"); axis("equal");
plot(arena_limit(1,[1,2,2,1,1]), arena_limit(2,[1,1,2,2,1]), 'k', 'LineWidth', 5) ;

p = load('../data/path.txt');
plot(p(:,2), p(:,3), 'b');



%c = load('../data/control.txt');
%plot(c(:,2), c(:,3), 'r');
%plot(c(:,2), c(:,3), 'or');

%m = load('../data/measure.txt');
%plot(m(:,2), m(:,3), '*g');
%plot(m(:,2), m(:,3), 'g');

%o = load('../data/odometry.txt');
%plot(o(:,6), o(:,7), 'k');
%for i=1:10:rows(o)
%  plot(o(i,[4,9]), o(i,[5,10]), 'g');
%end



%figure(); hold on;
%plot(o(:,2),'g');
%plot(o(:,7),'r');
%plot(o(:,12),'k');


figure(); hold on;
%plot(o(:,1),'k');
plot(diff(o(:,7)),'r');
plot(diff(o(:,8)),'g');
plot(diff(o(:,9)),'b');
%plot(o(:,3),'b');
%figure(); hold on;
%hist(diff(o(:,1)));
