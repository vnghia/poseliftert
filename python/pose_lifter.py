import torch
from tcn import TCN
from temporal_regression_head import TemporalRegressionHead


class PoseLifter(torch.nn.Module):
    def __init__(
        self,
        tcn_in_channels,
        head_in_channels,
        num_joints,
        stem_channels=1024,
        num_blocks=2,
        kernel_sizes=(3, 3, 3),
        dropout=0.25,
        causal=False,
        residual=True,
        use_stride_conv=False,
    ):
        super().__init__()

        self.tcn = TCN(
            tcn_in_channels,
            stem_channels,
            num_blocks,
            kernel_sizes,
            dropout,
            causal,
            residual,
            use_stride_conv,
        )

        self.keypoint_head = TemporalRegressionHead(head_in_channels, num_joints)

    def forward(self, x):
        return self.keypoint_head(self.tcn(x))

    def init_mmpose(self, weights_path):
        weights = torch.load(weights_path)["state_dict"]
        state_dict = {}
        assert len(weights) == len(
            self.state_dict()
        ), f"pre-trained weights have different size ({len(weights)} vs {len(self.state_dict())})"
        for name, (weight_name, weight) in zip(
            self.state_dict().keys(), weights.items()
        ):
            sub_names = name.split(".")[1:]
            sub_weight_names = weight_name.split(".")[1:]
            if len(sub_names) != len(sub_weight_names):
                sub_weight_names.pop(-3)
            assert (
                sub_names == sub_weight_names
            ), f"{name} and {weight_name} do not have similar structure."
            state_dict[name] = weight
        self.load_state_dict(state_dict)
