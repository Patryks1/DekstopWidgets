import PiHole from './widgets/PiHole/pihole';

export default function Widgets(props) {
  const [name, setName] = React.useState(props.name);
  return (
      <>
        <PiHole />
      </> 
  );
}