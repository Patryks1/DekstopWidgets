import React, { useState, useEffect  } from 'react';
import Card from '../components/Card';
import './PiHole.css';

function PiHole() {
  const [result, setResult] = useState('');
  const [error, setError] = useState(false);

  useEffect(() => {
    fetchPiholeData();
      setInterval(async () => {
        fetchPiholeData()
      }, 60000);
  });

  const fetchPiholeData = () => {
    setError(false);
    fetch("http://192.168.1.135/admin/api.php")
    .then((res) => res.json())
    .then(
      (result) => {
          result.fetchedTime = new Date().toLocaleString();
        setResult(result);
      },
      (error) => {
        setError(true);
      }
    );
  };

  const header = () => {
    return (
        <>
          <div className="headerRight">
          <p className="uniqueClients">
                {result && result.unique_clients}
              </p>
            <span className={result && result.status === "enabled" ? 'active' : 'inactive'}></span>
          </div>
        </>
      );
  };

  return (
    <>
        <Card name="PiHole" header={header()}>
          <div className="columnLeft">
            <p>Domains Being Blocked: </p>
            <p>Precent of blocked traffic: </p>
            <p>DNS queries Today: </p>
          </div>
          <div className="columnRight">
            <p>
              {result && result.domains_being_blocked}
            </p>
            <p>
              {result &&
                Math.round(result.ads_percentage_today * 100) /
                  100}{" "}
              %
            </p>
            <p>{result && result.dns_queries_today}</p>
            <p>{result && result.fetchedTime}</p>
          </div>
        </Card>
      </>
  );
};

export default PiHole;