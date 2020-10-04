import React, { useState } from 'react';
import './card.css';

function Card(props) {
    const [handleReferance, setHandleReferance] = useState(null);
    const [dragStartLeft, setdragStartLeft] = useState(0);
    const [dragStartTop, setdragStartTop] = useState(0);
    const [dragStartX, setdragStartX] = useState(0);
    const [dragStartY, setdragStartY] = useState(0);

    const setHandleRef = (ref) => {

        if (ref === null)
            return;

        setHandleReferance(ref);

        var elementDetails = JSON.parse(localStorage.getItem(props.name));
        if (elementDetails) {
            ref.style.left = elementDetails.left + 'px';
            ref.style.top = elementDetails.top + 'px';
        }
    };

    const initialiseDrag = (event) => {
        if (handleReferance == null)
            return;

            event.persist();

        const { target, clientX, clientY } = event;
        const { offsetTop, offsetLeft } = target;
        const { left, top } = handleReferance.getBoundingClientRect();

        setdragStartLeft(left - offsetLeft);
        setdragStartTop(top - offsetTop);
        setdragStartX(clientX);
        setdragStartY(clientY);

      window.addEventListener("mousemove", startDragging, false);
      window.addEventListener("mouseup", stopDragging, false);
    };

    const startDragging = ({ clientX, clientY }) => {
        if (handleReferance == null)
            return;

            handleReferance.style.top = (dragStartTop + clientY - dragStartY) + "px";
            handleReferance.style.left = (dragStartLeft + clientX - dragStartX) + "px";
    };

    const stopDragging = () => {
        if (handleReferance == null)
            return;

        window.removeEventListener("mousemove", startDragging, false);
        window.removeEventListener("mouseup", stopDragging, false);
  
        const { left, top, width, height } = handleReferance.getBoundingClientRect();
        localStorage.setItem(props.name, JSON.stringify({
          left: left,
          top: top,
          width: width,
          height: height
        }));
    };

    const header = () => {
        return (
            <div
            className="cardHeader"
              onMouseDown={initialiseDrag}
            >
              {props.header && props.header }
              <p className="title">{props.name}</p>
            </div>
        );
    };
  
    return (
        <>
            <div className="card" ref={setHandleRef}>
                {header()}
                <div className="content">{props.children}</div>
            </div>
        </>
    );
  };
  
  export default Card;
