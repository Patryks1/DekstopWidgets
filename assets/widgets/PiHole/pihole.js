import Card from "../shared/card";

export default class PiHole extends React.Component {
  constructor(props) {
    super(props);
    this.state = {
      result: null,
      error: false,
    };

    this.header = this.header.bind(this);
    this.fetchPiholeData = this.fetchPiholeData.bind(this);
  }

  fetchPiholeData() {
    fetch("http://192.168.1.135/admin/api.php")
    .then((res) => res.json())
    .then(
      (result) => {
          result.fetchedTime = new Date().toLocaleString();
        this.setState({ result: result });
      },
      (error) => {
        this.setState({ error: true });
      }
    );
  }

  componentDidMount() {
      this.fetchPiholeData();
      setInterval(async () => {
        this.fetchPiholeData()
      }, 60000);
  }

  header() {
    return (
      <>
        <div class="headerRight">
        <p class="uniqueClients">
              {this.state.result && this.state.result.unique_clients}
            </p>
          <span class={this.state.result && this.state.result.status === "enabled" ? 'active' : 'inactive'}></span>
        </div>
      </>
    );
  }
  render() {
    return (
      <>
        <Card name="PiHole" header={this.header()}>
          <div class="columnLeft">
            <p>Domains Being Blocked: </p>
            <p>Precent of blocked traffic: </p>
            <p>DNS queries Today: </p>
          </div>
          <div class="columnRight">
            <p>
              {this.state.result && this.state.result.domains_being_blocked}
            </p>
            <p>
              {this.state.result &&
                Math.round(this.state.result.ads_percentage_today * 100) /
                  100}{" "}
              %
            </p>
            <p>{this.state.result && this.state.result.dns_queries_today}</p>
            <p>{this.state.result && this.state.result.fetchedTime}</p>
          </div>
        </Card>
      </>
    );
  }
}