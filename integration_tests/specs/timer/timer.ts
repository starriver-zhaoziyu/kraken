describe('setTimeout', () => {
  it('resolve after 100ms', done => {
    const startTime = Date.now();
    setTimeout(() => {
      const duration = Date.now() - startTime;
      // 20ms delay accepted
      expect(duration - 100).toBeLessThanOrEqual(20);
      done();
    }, 100);
  });

  it('stop before resolved', () => {
    return new Promise((resolve, reject) => {
      let timer = setTimeout(() => {
        reject();
      }, 100);

      setTimeout(() => {
        resolve();
      }, 120);

      setTimeout(() => {
        clearTimeout(timer);
      }, 50);
    });
  });
});

describe('setInterval', function() {
  it('trigger 5 times and stop', () => {
    return new Promise((resolve, reject) => {
      let count = 0;
      let timer = setInterval(() => {
        count++;
        if (count > 5) {
          clearInterval(timer);
          resolve();
        }
      }, 10);
      setTimeout(() => {
        reject('setInterval execute time out!');
      }, 200);
    });
  });
});

describe('requestAnimationFrame', () => {
  it('async callback should have timestamp parameter', done => {
    requestAnimationFrame((timestamp) => {
      expect(typeof timestamp).toBe('number');
      done();
    });
  });
});

describe('clearTimeout', () => {
  it('should clear timeout', (done) => {
    const timer = setTimeout(() => {
      done.fail('clearTimeout not works.');
    }, 200);
    
    clearTimeout(timer);
    setTimeout(done, 250);
  });

  it('should accept non-numberic value', () => {
    var timer;
    clearTimeout(timer);
    
    timer = 'foo';
    clearTimeout(timer);

    timer = null;
    clearTimeout(timer);
  });
});