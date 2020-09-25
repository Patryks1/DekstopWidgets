export default class Card extends React.Component {
    constructor(props) {
      super(props);
      this.handleRef = null;
  
      this.startDragging = this.startDragging.bind(this);
      this.stopDragging = this.stopDragging.bind(this);
      this.setHandleRef = this.setHandleRef.bind(this);
      this.initialiseDrag = this.initialiseDrag.bind(this);
      this.header = this.header.bind(this);
    }
    
    setHandleRef(ref) {
      this.handleRef = ref;

      var elementDetails = JSON.parse(localStorage.getItem(this.props.name));
      if (elementDetails) {
        this.handleRef.style.left = elementDetails.left + 'px';
        this.handleRef.style.top = elementDetails.top + 'px';
      }
    }
  
    initialiseDrag(event) {
      const { target, clientX, clientY } = event;
      const { offsetTop, offsetLeft } = target;
      const { left, top } = this.handleRef.getBoundingClientRect();

      this.dragStartLeft = left - offsetLeft;
      this.dragStartTop = top - offsetTop;
      this.dragStartX = clientX;
      this.dragStartY = clientY;

      window.addEventListener("mousemove", this.startDragging, false);
      window.addEventListener("mouseup", this.stopDragging, false);
    }
  
    startDragging({ clientX, clientY }) {
        this.handleRef.style.top = (this.dragStartTop + clientY - this.dragStartY) + "px";
        this.handleRef.style.left = (this.dragStartLeft + clientX - this.dragStartX) + "px";
    }
  
    stopDragging() {
      window.removeEventListener("mousemove", this.startDragging, false);
      window.removeEventListener("mouseup", this.stopDragging, false);

      const { left, top, width, height } = this.handleRef.getBoundingClientRect();
      localStorage.setItem(this.props.name, JSON.stringify({
        left: left,
        top: top,
        width: width,
        height: height
      }));
    }
    
    header() {
        return (
            <div
              class="cardHeader"
              onMouseDown={this.initialiseDrag}
            >
              {this.props.header && this.props.header }
              <p class="title">{this.props.name}</p>
            </div>
        );
    }

    render() {
      return (
        <>
          <div class="card" ref={this.setHandleRef}>
            {this.header()}
            <div class="content">{this.props.children}</div>
          </div>
        </>
      );
    }
  }
  