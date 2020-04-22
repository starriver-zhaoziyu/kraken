describe('Position absolute', () => {
  it('001', async () => {
    var container = document.createElement('div');
    var div1 = document.createElement('div');
    var div2 = document.createElement('span');

    container.appendChild(div1);
    container.appendChild(div2);
    document.body.appendChild(container);

    container.style.width = '300px';
    container.style.height = '800px';
    container.style.backgroundColor = '#999';

    div1.style.position = 'absolute';
    div1.style.width = '100px';
    div1.style.height = '200px';
    div1.style.backgroundColor = 'red';

    div2.style.position = 'absolute';
    div2.style.width = '100px';
    div2.style.height = '100px';
    div2.style.top = '50px';
    div2.style.backgroundColor = 'green';

    container.style.marginLeft = '50px';
    container.style.position = 'relative';
    container.style.top = '100px';

    await matchScreenshot();
  });

  it('should be a green square below', async done => {
    let parent = create('div', {
      width: '150px',
      height: '150px',
      backgroundColor: 'green',
    });
    let child = create('div', {
      width: '150px',
      height: '150px',
      backgroundColor: 'white',
      position: 'absolute',
    });
    append(parent, child);
    append(BODY, parent);
    await matchElementImageSnapshot(parent);

    requestAnimationFrame(async () => {
      child.style.left = '150px';
      await matchElementImageSnapshot(parent);
      done();
    });
  });

  it('with no left following inline element', async () => {
    let div1 = create('div', {
      border: '1px solid black',
      padding: '100px',
      position: 'relative',
    });
    let div2 = create('div', {
      border: '1px solid black',
      padding: '50px',
      backgroundColor: 'green',
    });
    append(div2, createText('inline'));
    const span = create('span', {
      backgroundColor: 'blue',
      height: '100px',
      width: '100px',
      top: '50px',
      position: 'absolute'
    });
    append(span, createText('absolute with no left'));
    append(div2, span);
    append(BODY, div1);
    append(div1, div2);
    await matchScreenshot();
  });

  it('with no top following inline element', async () => {
    let div1 = create('div', {
      border: '1px solid black',
      padding: '100px',
      position: 'relative',
    });
    let div2 = create('div', {
      border: '1px solid black',
      padding: '50px',
      backgroundColor: 'green',
    });
    append(div2, createText('inline'));
    const span = create('span', {
      backgroundColor: 'blue',
      height: '100px',
      width: '100px',
      left: '50px',
      position: 'absolute'
    });
    append(span, createText('absolute with no top'));
    append(div2, span);
    append(BODY, div1);
    append(div1, div2);
    await matchScreenshot();
  });

  it('with no left following block element', async () => {
    let div1 = create('div', {
      border: '1px solid black',
      padding: '100px',
      position: 'relative',
    });
    let div2 = create('div', {
      border: '1px solid black',
      padding: '50px',
      backgroundColor: 'green',
    });
    let div3 = create('div', {});
    append(div3, createText('block'));
    append(div2, div3);
    const span = create('span', {
      backgroundColor: 'blue',
      height: '100px',
      width: '100px',
      top: '50px',
      position: 'absolute'
    });
    append(span, createText('absolute with no left'));
    append(div2, span);
    append(BODY, div1);
    append(div1, div2);
    await matchScreenshot();
  });

  it('with no top following block element', async () => {
    let div1 = create('div', {
      border: '1px solid black',
      padding: '100px',
      position: 'relative',
    });
    let div2 = create('div', {
      border: '1px solid black',
      padding: '50px',
      backgroundColor: 'green',
    });
    let div3 = create('div', {});
    append(div3, createText('block'));
    append(div2, div3);
    const span = create('span', {
      backgroundColor: 'blue',
      height: '100px',
      width: '100px',
      left: '50px',
      position: 'absolute'
    });
    append(span, createText('absolute with no top'));
    append(div2, span);
    append(BODY, div1);
    append(div1, div2);
    await matchScreenshot();
  });

  it('works with dynamic change bottom property', async (done) => {
    const div = document.createElement('div');
    div.style.width = '300px';
    div.style.height = '300px';
    div.style.backgroundColor = 'red';
    div.style.position = 'absolute';
    
    setTimeout(async () => {
      div.style.bottom = '100px';
      await matchScreenshot();
    }, 100);

    setTimeout(async () => {
      div.style.bottom = '-200px';
      await matchScreenshot();
      done();
    }, 300);
    
    document.body.appendChild(div);
    await matchScreenshot();
  });

  it('works with dynamic change width property', async (done) => {
    const div = document.createElement('div');
    div.style.width = '300px';
    div.style.height = '300px';
    div.style.backgroundColor = 'red';
    div.style.position = 'absolute';
    
    setTimeout(async () => {
      div.style.width = '100px';
      await matchScreenshot();
    }, 100);

    setTimeout(async () => {
      div.style.width = '400px';
      await matchScreenshot();
      done();
    }, 300);
    
    document.body.appendChild(div);
    await matchScreenshot();
  });

  it('works with dynamic change height property', async (done) => {
    const div = document.createElement('div');
    div.style.width = '300px';
    div.style.height = '300px';
    div.style.backgroundColor = 'red';
    div.style.position = 'absolute';
    
    setTimeout(async () => {
      div.style.height = '100px';
      await matchScreenshot();
    }, 100);

    setTimeout(async () => {
      div.style.height = '400px';
      await matchScreenshot();
      done();
    }, 300);
    
    document.body.appendChild(div);
    await matchScreenshot();
  });

  it('works with dynamic change top property', async (done) => {
    const div = document.createElement('div');
    div.style.width = '300px';
    div.style.height = '300px';
    div.style.backgroundColor = 'red';
    div.style.position = 'absolute';
    
    setTimeout(async () => {
      div.style.top = '100px';
      await matchScreenshot();
    }, 100);

    setTimeout(async () => {
      div.style.top = '-50px';
      await matchScreenshot();
      done();
    }, 300);
    
    document.body.appendChild(div);
    await matchScreenshot();
  });

  it('works with dynamic change left property', async (done) => {
    const div = document.createElement('div');
    div.style.width = '300px';
    div.style.height = '300px';
    div.style.backgroundColor = 'red';
    div.style.position = 'absolute';
    
    setTimeout(async () => {
      div.style.left = '100px';
      await matchScreenshot();
    }, 100);

    setTimeout(async () => {
      div.style.left = '-50px';
      await matchScreenshot();
      done();
    }, 300);
    
    document.body.appendChild(div);
    await matchScreenshot();
  });

  it('works with dynamic change right property', async (done) => {
    const div = document.createElement('div');
    div.style.width = '300px';
    div.style.height = '300px';
    div.style.backgroundColor = 'red';
    div.style.position = 'absolute';
    
    setTimeout(async () => {
      div.style.right = '100px';
      await matchScreenshot();
    }, 100);

    setTimeout(async () => {
      div.style.right = '-50px';
      await matchScreenshot();
      done();
    }, 300);
    
    document.body.appendChild(div);
    await matchScreenshot();
  });
});